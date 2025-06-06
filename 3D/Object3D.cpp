#include "Engine/LOG.hpp"
#include "Object3D.hpp"
#include "Engine/GameEngine.hpp"

static void set_perspective_transform()
{
    ALLEGRO_BITMAP* bmp = al_get_backbuffer(al_get_current_display());
    ALLEGRO_TRANSFORM p;
    float width = al_get_bitmap_width(bmp);
    float height = al_get_bitmap_height(bmp);
    al_set_target_bitmap(bmp);
    al_identity_transform(&p);
    al_perspective_transform(&p, -width / 2, -height / 2, width / 2, width / 2, height / 2, 9000);
    al_use_projection_transform(&p);
}
float a = 0;
static ALLEGRO_TRANSFORM set_camera() {
    ALLEGRO_TRANSFORM t;
    Engine::Point vec(0, 3, 6);
    vec = vec.Normalize();
    vec = vec * 1200;
    al_build_camera_transform(&t,
        vec.x, vec.y, vec.z,
        0, 0, 0,
        0, 1, 0);
    set_perspective_transform();
    //a += 10;
    if (a >= 2000) a = -2000;
    al_translate_transform_3d(&t, -800 + a, -416, 0);
    al_rotate_transform_3d(&t, 1, 0, 0, -ALLEGRO_PI / 24);
    al_use_transform(&t);
    return t;
}
float i = 0;
float theta = 0;
void DrawCube(std::vector<ALLEGRO_VERTEX> vertices, std::vector<int> indices, ALLEGRO_BITMAP* texture) {
    ALLEGRO_STATE state;
    ALLEGRO_TRANSFORM t;

    al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP |
        ALLEGRO_STATE_TRANSFORM |
        ALLEGRO_STATE_PROJECTION_TRANSFORM);
    al_set_render_state(ALLEGRO_DEPTH_TEST, true);
    ALLEGRO_TRANSFORM defaultTrans = set_camera();

    i += 0.05;
    theta += 0.05;
    if (i >= 1) i = 0;
    if (theta >= ALLEGRO_PI * 2) theta = 0;
    Engine::Point mouse = Engine::GameEngine::GetInstance().GetMousePosition();
    //al_transform_coordinates(&t, &mouse.x, &mouse.y);  
    for (int x = 0; x < 14; x++) {
        for (int y = 0; y < 8; y++) {
            al_identity_transform(&t); 
            al_scale_transform_3d(&t, 50, 50, 50);
            al_translate_transform_3d(&t, x * 120, y*120, -50 * ((x * y) % 3 == 0));
            al_compose_transform(&t, &defaultTrans);
            al_use_transform(&t);
            al_draw_indexed_prim(vertices.data(), nullptr, texture, indices.data(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);
        }
    }
    
    al_use_transform(&defaultTrans);
    al_draw_rectangle(0, 0, 1600, 832, al_map_rgb_f(0, 0, 1), 10);
    al_draw_circle(800, 416, 10, al_map_rgb_f(1, 0, 0), 5);

    al_set_render_state(ALLEGRO_DEPTH_TEST, false);
    al_restore_state(&state);
}


Object3D::Object3D(std::string gltfFile, int x, int y, float scaleX, float scaleY, int anchorX, int anchorY)
{
	std::string err, warn;
    tinygltf::TinyGLTF loader;
	bool res = loader.LoadBinaryFromFile(&model, &err, &warn, gltfFile);
    if (!warn.empty()) {
        Engine::LOG(Engine::WARN) << "WARN: " << warn;
    }

    if (!err.empty()) {
        Engine::LOG(Engine::WARN) << err;
        return;
    }

    if (!res) {
        Engine::LOG(Engine::WARN) << "Failed to load file: " << gltfFile;
        return;
    }
    else
        Engine::LOG(Engine::INFO) << "Loaded glTF: " << gltfFile;

    vertices.clear();

    tinygltf::Image image = model.images[0];
    texture = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(image.width, image.height), al_destroy_bitmap);
    ALLEGRO_LOCKED_REGION* al_bmp_mem = al_lock_bitmap(texture.get(), ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_WRITEONLY);
    memcpy(al_bmp_mem->data, image.image.data(), image.image.size());
    al_unlock_bitmap(texture.get());

    for (auto mesh : model.meshes) {
        for (auto primitive : mesh.primitives) {
            std::vector<float> verts = getFromAccessor<float>(primitive.attributes["POSITION"]);
            std::vector<float> uv = getFromAccessor<float>(primitive.attributes["TEXCOORD_0"]);
            std::vector<uint16_t> index = getFromAccessor<uint16_t>(primitive.indices);
            for (int i = 0; i < verts.size() / 3; i++) {
                vertices.push_back({ verts[i * 3 + 0], verts[i * 3 + 1] , verts[i * 3 + 2],
                                     uv[i * 2] * image.width, uv[i * 2 + 1] * image.height,
                                     al_map_rgba_f(1,1,1,1) });
            }
            indices.resize(index.size());
            for (int i = 0; i < indices.size(); i++) indices[i] = index[i];
        }
    }
}

template<typename T>
inline std::vector<T> Object3D::getFromAccessor(int accessorID) const
{
	const tinygltf::Accessor& accessor = model.accessors[accessorID];
	const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
	// bufferView byteoffset + accessor byteoffset tells you where the actual position data is within the buffer. From there
	// you should already know how the data needs to be interpreted.
	const T* pointer = reinterpret_cast<const T*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
    int typeSize = accessor.type;
    if (typeSize == TINYGLTF_TYPE_SCALAR) typeSize = 1;
	return std::vector<T>(pointer, pointer + accessor.count * typeSize);
}


void Object3D::Draw() const
{

    DrawCube(vertices, indices, texture.get());
}

void Object3D::Update(float delta)
{
    
}
