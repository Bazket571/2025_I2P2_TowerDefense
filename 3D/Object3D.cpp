#include "Engine/LOG.hpp"
#include "Object3D.hpp"

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
}

void Object3D::Update(float delta)
{
    vertices.clear();
    for (auto mesh : model.meshes) {
        for (auto primitive : mesh.primitives) {
            std::vector<float> verts = getFromAccessor<float>(primitive.attributes["POSITION"]);
            std::vector<float> uv = getFromAccessor<float>(primitive.attributes["TEXCOORD_0"]);
            std::vector<uint16_t> indices = getFromAccessor<uint16_t>(primitive.indices);
            for (int i = 0; i < verts.size() / 3; i++) {
                vertices.push_back({ verts[i * 3 + 0], verts[i * 3 + 1] , verts[i * 3 + 2],
                                     uv[i * 2], uv[i*2 + 1],
                                     al_map_rgba_f(1,1,1,1)});
            }
        }
    }
}
