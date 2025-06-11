#include "Engine/spine/spine.hpp"

#include <Engine/Resources.hpp>

const std::string Engine::SpineSprite::spinePathPrefix = "Resource/spine/";
const int blendModesParam[2][4][3] = {
	{
		{ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA},		//Normal
		{ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE},					//Additive
		{ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_INVERSE_ALPHA},	//Mulitply
		{ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_SRC_COLOR}		//Screen
	},
	{
		{ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA},			//Normal
		{ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE},					//Additive
		{ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_INVERSE_ALPHA},	//Mulitply
		{ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_SRC_COLOR}		//Screen
	} 
};

spine::SpineExtension* spine::getDefaultExtension() {
	return new spine::DefaultSpineExtension();
}

void Engine::SpineTextureLoader::load(spine::AtlasPage& page, const spine::String& path)
{
	std::string pathStr(path.buffer());
	ALLEGRO_BITMAP* texture = Engine::Resources::GetInstance().GetBitmap(pathStr, "").get();
	if (!texture) return;
	page.setRendererObject(texture);
}

void Engine::SpineTextureLoader::unload(void* texture)
{
	//Texture will get destroyed by the smart pointer.
}

Engine::SpineSprite::SpineSprite(std::string skeletonFile, std::string atlasFile,
	float x, float y, float scale, float anchorX, float anchorY,
	std::string pathPrefix) :
	timeScale(1),
	usePremultipliedAlpha(false),
	worldVertices(), clipper(),
	IObject(x, y, 0, 0, anchorX, anchorY)
{
	Engine::SpineTextureLoader textureLoader;
	spine::Atlas *atlas = new spine::Atlas((pathPrefix + atlasFile).c_str(), &textureLoader);
	spine::SkeletonBinary skeletonBinary(atlas);
	skeletonBinary.setScale(scale);
	spine::SkeletonData* skeletonData = skeletonBinary.readSkeletonDataFile((pathPrefix + skeletonFile).c_str());
	setUsePremultipliedAlpha(true);

	spine::Bone::setYDown(true);
	skeleton = new spine::Skeleton(skeletonData);
	//skeleton->setPosition(Position.x, Position.y);
	state = new spine::AnimationState(new spine::AnimationStateData(skeletonData));
	Scale = { scale, scale, 1 };
	quadIndices.add(0);
	quadIndices.add(1);
	quadIndices.add(2);
	quadIndices.add(2);
	quadIndices.add(3);
	quadIndices.add(0);
}

Engine::SpineSprite::~SpineSprite()
{
	if (ownsAnimationStateData) delete state->getData();
	delete skeleton;
	delete state;
}

void Engine::SpineSprite::Update(float deltaTime)
{
	state->update(deltaTime * timeScale);
	state->apply(*skeleton);
	skeleton->update(deltaTime * timeScale);
	//skeleton->setPosition(Position.x, Position.y);
	skeleton->updateWorldTransform();
}

inline ALLEGRO_COLOR toAllegroColor(uint32_t color) {
	return al_map_rgba((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, (color >> 24) & 0xFF);
}

//https://github.com/EsotericSoftware/spine-runtimes/blob/3.8/spine-sfml/cpp/src/spine/spine-sfml.cpp
void Engine::SpineSprite::Draw() const
{
	int prevOp, prevSrc, prevDst;
	al_get_blender(&prevOp, &prevSrc, &prevDst);
	vertexArray.clear();

	// Early out if skeleton is invisible
	if (skeleton->getColor().a == 0) return;

	//if (vertexEffect != NULL) vertexEffect->begin(*skeleton);

	ALLEGRO_VERTEX vertex;
	vertex.z = 0;
	ALLEGRO_BITMAP* texture;
	for (unsigned i = 0; i < skeleton->getSlots().size(); ++i) {
		spine::Slot &slot = *skeleton->getDrawOrder()[i];
		spine::Attachment *attachment = slot.getAttachment();
		if (!attachment) continue;

		// Early out if the slot color is 0 or the bone is not active
		if (slot.getColor().a == 0 || !slot.getBone().isActive()) {
			clipper.clipEnd(slot);
			continue;
		}

		spine::Vector<float> *vertices = &worldVertices;
		int verticesCount = 0;
		spine::Vector<float> *uvs = NULL;
		spine::Vector<uint16_t> *indices = NULL;
		int indicesCount = 0;
		spine::Color *attachmentColor;

		if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
			spine::RegionAttachment *regionAttachment = (spine::RegionAttachment*)attachment;
			attachmentColor = &regionAttachment->getColor();

			// Early out if the slot color is 0
			if (attachmentColor->a == 0) {
				clipper.clipEnd(slot);
				continue;
			}

			worldVertices.setSize(8, 0);
			regionAttachment->computeWorldVertices(slot.getBone(), worldVertices, 0, 2);
			verticesCount = 4;
			uvs = &regionAttachment->getUVs();
			indices = &quadIndices;
			indicesCount = 6;
			texture = (ALLEGRO_BITMAP*)((spine::AtlasRegion*) regionAttachment->getRendererObject())->page->getRendererObject();

		} else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
			spine::MeshAttachment *mesh = (spine::MeshAttachment*)(attachment);
			attachmentColor = &mesh->getColor();

			// Early out if the slot color is 0
			if (attachmentColor->a == 0) {
				clipper.clipEnd(slot);
				continue;
			}

			worldVertices.setSize(mesh->getWorldVerticesLength(), 0);
			texture = (ALLEGRO_BITMAP*)((spine::AtlasRegion*) mesh->getRendererObject())->page->getRendererObject();
			mesh->computeWorldVertices(slot, 0, mesh->getWorldVerticesLength(), worldVertices, 0, 2);
			verticesCount = mesh->getWorldVerticesLength() >> 1;
			uvs = &mesh->getUVs();
			indices = &mesh->getTriangles();
			indicesCount = mesh->getTriangles().size();

		} else if (attachment->getRTTI().isExactly(spine::ClippingAttachment::rtti)) {
			spine::ClippingAttachment *clip = (spine::ClippingAttachment*)(slot.getAttachment());
			clipper.clipStart(slot, clip);
			continue;
		} else continue;

		float r = skeleton->getColor().r * slot.getColor().r * attachmentColor->r;
		float g = skeleton->getColor().g * slot.getColor().g * attachmentColor->g;
		float b = skeleton->getColor().b * slot.getColor().b * attachmentColor->b;
		float a = skeleton->getColor().a * slot.getColor().a * attachmentColor->a;

		spine::Color light(r, g, b, a);

		if (clipper.isClipping()) {
			clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
			vertices = &clipper.getClippedVertices();
			verticesCount = clipper.getClippedVertices().size() >> 1;
			uvs = &clipper.getClippedUVs();
			indices = &clipper.getClippedTriangles();
			indicesCount = clipper.getClippedTriangles().size();
		}

		//Vector2u size = texture->getSize();
		int bitmapHeight = al_get_bitmap_height(texture);
		int bitmapWidth  = al_get_bitmap_width(texture);
		//Remove vertex effect
		/* if (vertexEffect != 0) {
			tempColors.clear();
			for (int ii = 0; ii < verticesCount; ii++) {
				spine::Color vertexColor = light;
				spine::Color dark;
				dark.r = dark.g = dark.b = dark.a = 0;
				int index = ii << 1;
				float x = (*vertices)[index];
				float y = (*vertices)[index + 1];
				float u = (*uvs)[index];
				float v = (*uvs)[index + 1];
				vertexEffect->transform(x, y, u, v, vertexColor, dark);
				(*vertices)[index] = x;
				(*vertices)[index + 1] = y;
				tempColors.add(vertexColor);
			}

			for (int ii = 0; ii < indicesCount; ++ii) {
				int index = (*indices)[ii] << 1;
				vertex.x = (*vertices)[index];
				vertex.y = (*vertices)[index + 1];
				vertex.u = (*uvs)[index] * bitmapWidth;
				vertex.v = (*uvs)[index + 1] * bitmapHeight;
				spine::Color vertexColor = tempColors[index >> 1];
				vertex.color = al_map_rgba_f(vertexColor.r, vertexColor.g, vertexColor.b, vertexColor.a);
				vertexArray.push_back(vertex);
			}
		} else {*/
		//Mapping indexed vertex array to normal vertex array
		for (int ii = 0, j = 0; ii < verticesCount; ii++, j += 2) {
			vertex.x = (*vertices)[j];
			vertex.y = (*vertices)[j + 1];
			vertex.z = 0;
			vertex.u = (*uvs)[j] * bitmapWidth;
			vertex.v = (*uvs)[j + 1] * bitmapHeight;
			vertex.color = al_map_rgba_f(r, g, b, a);
			vertexArray.push_back(vertex);
		}
		//Copy indicies
		std::vector<int> drawIndicies(indicesCount);
		for(int in = 0; in < indicesCount; in++)
			drawIndicies[in] = (*indices)[in];

		//Draw
		
		al_set_blender(
			blendModesParam[usePremultipliedAlpha][slot.getData().getBlendMode()][0],
			blendModesParam[usePremultipliedAlpha][slot.getData().getBlendMode()][1],
			blendModesParam[usePremultipliedAlpha][slot.getData().getBlendMode()][2]);	
		al_draw_indexed_prim(vertexArray.data(), NULL, texture, drawIndicies.data(), indicesCount, ALLEGRO_PRIM_TRIANGLE_LIST);
		vertexArray.clear();
		clipper.clipEnd(slot);
	}
	//target.draw(*vertexArray, states);
	clipper.clipEnd();

	//if (vertexEffect != 0) vertexEffect->end();	
	al_set_blender(prevOp, prevSrc, prevDst);
	vertexArray.clear();
	IObject::Draw();
}
