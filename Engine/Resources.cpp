#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <list>
#include <string>
#include <utility>
#include <lunasvg.h>

#include "Engine/Allegro5Exception.hpp"
#include "LOG.hpp"
#include "Resources.hpp"
#include "3D/Object3D.hpp"
#include "tiny_gltf.h"


// #include "Resources.hpp"

namespace Engine {
    const std::string Resources::bitmapPathPrefix = "Resource/images/";
    const std::string Resources::fontPathPrefix = "Resource/fonts/";
    const std::string Resources::samplePathPrefix = "Resource/audios/";

    std::vector<ALLEGRO_VERTEX_ELEMENT> Resources::vertexElems = {
        {ALLEGRO_PRIM_POSITION, ALLEGRO_PRIM_FLOAT_3, offsetof(Object3D::Vertex, x)},
        {ALLEGRO_PRIM_TEX_COORD, ALLEGRO_PRIM_FLOAT_2, offsetof(Object3D::Vertex, u)},
        {ALLEGRO_PRIM_USER_ATTR, ALLEGRO_PRIM_FLOAT_3, offsetof(Object3D::Vertex, nx)}, //Normal
        {ALLEGRO_PRIM_COLOR_ATTR, 0, offsetof(Object3D::Vertex, color)},
        {0,0,0}
    };
    std::shared_ptr<ALLEGRO_VERTEX_DECL> Resources::vertexDecl = nullptr;


    Resources::Resources() {
        vertexDecl = std::shared_ptr<ALLEGRO_VERTEX_DECL>(al_create_vertex_decl(vertexElems.data(), sizeof(Vertex)), al_destroy_vertex_decl);
    };
    Resources::~Resources() {
        try {
            ReleaseUnused();
        } catch (...) {}
    }

    void Resources::ReleaseUnused() {
        // A better way may be to count the memory usage and release unused resources
        // when the total resource memory exceeds a certain threshold. However, we'll
        // just keep it simple here and only release unused resources in GameEngine when
        // changing between scenes.
        for (auto it = bitmaps.begin(); it != bitmaps.end();) {
            if (it->second.use_count() == 1) {
                LOG(INFO) << "Destroyed Resource<image>: " << it->first;
                it = bitmaps.erase(it);
            } else ++it;
        }
        for (auto it = fonts.begin(); it != fonts.end();) {
            if (it->second.use_count() == 1) {
                LOG(INFO) << "Destroyed Resource<font>: " << it->first;
                it = fonts.erase(it);
            } else ++it;
        }
        for (auto it = sample_instance_pairs.begin(); it != sample_instance_pairs.end();) {
            if (it->second.first.use_count() == 1) {
                LOG(INFO) << "Destroyed<sample_instance>: " << it->first;
                it = sample_instance_pairs.erase(it);
            } else ++it;
        }
        // Stops playing samples whose instance isn't referenced.
        for (auto it = samples.begin(); it != samples.end();) {
            if (it->second.use_count() == 1) {
                LOG(INFO) << "Destroyed Resource<audio>: " << it->first;
                it = samples.erase(it);
            } else ++it;
        }
    }
    
    std::shared_ptr<ALLEGRO_BITMAP> Resources::GetBitmap(std::string name, std::string pathPrefix) {
        if(name.substr(name.length() - 3) == "svg") return Resources::GetSVG(name);

        if (bitmaps.count(name) != 0)
            return bitmaps[name];
        std::string bitmapPath = pathPrefix + name;
        ALLEGRO_BITMAP *bmp = al_load_bitmap(bitmapPath.c_str());
        if (!bmp) throw Allegro5Exception(("failed to load image: " + bitmapPath).c_str());
        LOG(INFO) << "Loaded Resource<image>: " << bitmapPath;
        bitmaps[name] = std::shared_ptr<ALLEGRO_BITMAP>(bmp, al_destroy_bitmap);
        return bitmaps[name];
    }

    template<typename T>
    inline std::vector<T> getFromAccessor(int accessorID, const tinygltf::Model& model)
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

    std::pair<std::shared_ptr<ALLEGRO_VERTEX_BUFFER>, std::shared_ptr<ALLEGRO_INDEX_BUFFER>> Engine::Resources::GetModel(std::string gltfFile, std::shared_ptr<ALLEGRO_BITMAP>& retTexture) {
        if (models.count(gltfFile) != 0) {
            retTexture = bitmaps[gltfFile];
            return models[gltfFile];
        }
        
        tinygltf::Model model;
        std::string err, warn;
        tinygltf::TinyGLTF loader;
        bool res = loader.LoadBinaryFromFile(&model, &err, &warn, gltfFile);
        if (!warn.empty()) {
            Engine::LOG(Engine::WARN) << "WARN: " << warn;
        }
        if (!err.empty()) {
            Engine::LOG(Engine::WARN) << err;
            return { nullptr, nullptr };
        }
        if (!res) {
            Engine::LOG(Engine::WARN) << "Failed to load file: " << gltfFile;
            return { nullptr, nullptr };
        }
        else {
            Engine::LOG(Engine::INFO) << "Loaded glTF: " << gltfFile;
        }
        tinygltf::Image image = model.images[0];
        //al_set_new_bitmap_depth(0);
        retTexture = Engine::Resources::GetInstance().GetMemory(gltfFile, image.image.data(), image.width, image.height, image.width * 4, ALLEGRO_PIXEL_FORMAT_ABGR_8888);

        //Load 3d model, only accept 1 primitive as we only have a cube
        auto primitive = model.meshes[0].primitives[0];
        std::vector<float> verts = getFromAccessor<float>(primitive.attributes["POSITION"], model);
        std::vector<float> normals = getFromAccessor<float>(primitive.attributes["NORMAL"], model);
        std::vector<float> uv = getFromAccessor<float>(primitive.attributes["TEXCOORD_0"], model);
        std::vector<uint16_t> index = getFromAccessor<uint16_t>(primitive.indices, model);
        std::vector<Vertex> vertices;
        for (int i = 0; i < verts.size() / 3; i++) {
            vertices.push_back({ verts[i * 3 + 0], verts[i * 3 + 1] , verts[i * 3 + 2],
                                    uv[i * 2], uv[i * 2 + 1],
                                    normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2],
                                    al_map_rgba_f(1,1,1,1) });
        }
        models[gltfFile].first = std::shared_ptr<ALLEGRO_VERTEX_BUFFER>(
            al_create_vertex_buffer(vertexDecl.get(), vertices.data(), vertices.size(), ALLEGRO_PRIM_BUFFER_STATIC),
            al_destroy_vertex_buffer);

        models[gltfFile].second = std::shared_ptr<ALLEGRO_INDEX_BUFFER>(
            al_create_index_buffer(sizeof(index[0]), index.data(), index.size(), ALLEGRO_PRIM_BUFFER_STATIC),
            al_destroy_index_buffer);

        return models[gltfFile];
    }
    std::shared_ptr<ALLEGRO_BITMAP> Resources::GetMemory(std::string name, uint8_t* data, int width, int height, int stride, int pixelFormat, std::string announce) {
        if (bitmaps.count(name) != 0)
            return bitmaps[name];
        if (announce.empty()) announce = name;
        ALLEGRO_BITMAP* al_bmp = al_create_bitmap(width, height);
        ALLEGRO_LOCKED_REGION* al_bmp_mem = al_lock_bitmap(al_bmp, pixelFormat, ALLEGRO_LOCK_WRITEONLY);

        for (int i = 0; i < height; i++) {
            memcpy((uint8_t*)al_bmp_mem->data + i * al_bmp_mem->pitch,
                data + i * stride,
                width * al_bmp_mem->pixel_size);
        }
        al_unlock_bitmap(al_bmp);

        LOG(INFO) << "Loaded Resource<image>: " << announce << " scaled to " << width << "x" << height;
        bitmaps[name] = std::shared_ptr<ALLEGRO_BITMAP>(al_bmp, al_destroy_bitmap);

        return bitmaps[name];
    }
    std::shared_ptr<ALLEGRO_BITMAP> Resources::GetSVG(std::string name, int width, int height, std::string pathPrefix){
        std::string whiteFill = R"CSS(
            *{
                fill: #FFFFFFFF;
            }
        )CSS";

        std::string bitmapPath = pathPrefix + name;
        if (width >= 0 || height >= 0) {
            name += '?' + std::to_string(width) + 'x' + std::to_string(height);
        }

        if (bitmaps.count(name) != 0)
            return bitmaps[name];

        auto document = lunasvg::Document::loadFromFile(bitmapPath);
        if(!document) throw Allegro5Exception(("failed to load image: " + bitmapPath).c_str());
        document->applyStyleSheet(whiteFill);
        auto bitmap = document->renderToBitmap(width, height, 0x00000001); //Making alpha = 1 to prevent engine marking it invisible
        
        return GetMemory(name, bitmap.data(), bitmap.width(), bitmap.height(), bitmap.stride(), ALLEGRO_PIXEL_FORMAT_ABGR_8888, bitmapPath);
    }
    std::shared_ptr<ALLEGRO_BITMAP> Resources::GetBitmap(std::string name, int width, int height, std::string pathPrefix)
    {
        if(name.substr(name.length() - 3) == "svg") return Resources::GetSVG(name, width, height);

        std::string idx = name + '?' + std::to_string(width) + 'x' + std::to_string(height);
        if (bitmaps.count(idx) != 0)
            return bitmaps[idx];
        std::string bitmapPath = pathPrefix + name;
        ALLEGRO_BITMAP *bmp = al_load_bitmap(bitmapPath.c_str());
        if (!bmp) throw Allegro5Exception(("failed to load image: " + bitmapPath).c_str());

        ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(width, height);
        ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();
        if (!resized_bmp) throw Allegro5Exception(("failed to create bitmap when creating resized image: " + bitmapPath).c_str());
        al_set_target_bitmap(resized_bmp);
        al_draw_scaled_bitmap(bmp, 0, 0,
                              al_get_bitmap_width(bmp),
                              al_get_bitmap_height(bmp),
                              0, 0, width, height, 0);
        al_set_target_bitmap(prev_target);
        al_destroy_bitmap(bmp);

        LOG(INFO) << "Loaded Resource<image>: " << bitmapPath << " scaled to " << width << "x" << height;
        bitmaps[idx] = std::shared_ptr<ALLEGRO_BITMAP>(resized_bmp, al_destroy_bitmap);
        return bitmaps[idx];
    }
    std::shared_ptr<ALLEGRO_FONT> Resources::GetFont(std::string name, int fontSize) {
        std::string idx = name + '?' + std::to_string(fontSize);
        if (fonts.count(idx) != 0)
            return fonts[idx];
        std::string fontPath = fontPathPrefix + name;
        ALLEGRO_FONT *font = al_load_font(fontPath.c_str(), fontSize, 0);
        if (!font)
            throw Allegro5Exception(("failed to load font: " + fontPath).c_str());
        LOG(INFO) << "Loaded Resource<font>: " << fontPath << " with size " << fontSize;
        fonts[idx] = std::shared_ptr<ALLEGRO_FONT>(font, al_destroy_font);
        return fonts[idx];
    }
    std::shared_ptr<ALLEGRO_SAMPLE> Resources::GetSample(std::string name) {
        if (samples.count(name) != 0)
            return samples[name];
        std::string samplePath = samplePathPrefix + name;
        ALLEGRO_SAMPLE *sample = al_load_sample(samplePath.c_str());
        if (!sample)
            throw Allegro5Exception(("failed to load audio: " + samplePath).c_str());
        LOG(INFO) << "Loaded Resource<audio>: " << samplePath;
        samples[name] = std::shared_ptr<ALLEGRO_SAMPLE>(sample, al_destroy_sample);
        return samples[name];
    }
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> Resources::GetSampleInstance(std::string name) {
        std::shared_ptr<ALLEGRO_SAMPLE> sample = GetSample(name);
        ALLEGRO_SAMPLE_INSTANCE *sample_instance = al_create_sample_instance(sample.get());
        std::string samplePath = samplePathPrefix + name;
        if (!sample_instance)
            throw Allegro5Exception(("failed to create sample instance: " + samplePath).c_str());
        LOG(INFO) << "Created<sample_instance>: " << samplePath;
        std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> ptr = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>(sample_instance, al_destroy_sample_instance);
        sample_instance_pairs[name] = std::make_pair(ptr, sample);
        return ptr;
    }
    Resources &Resources::GetInstance() {
        // The classic way to lazy initialize a Singleton.
        static Resources instance;
        return instance;
    }
}
