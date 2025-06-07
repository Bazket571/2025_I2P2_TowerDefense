#include "Object3D.hpp"

static ALLEGRO_TRANSFORM perspective_transform()
{
    ALLEGRO_BITMAP* bmp = al_get_target_bitmap();
    ALLEGRO_TRANSFORM p;
    float width = al_get_bitmap_width(bmp);
    float height = al_get_bitmap_height(bmp);
    al_identity_transform(&p);
    al_perspective_transform(&p, -width / 2, -height / 2, width / 2, width / 2, height / 2, 9000);
    return p;
}
static ALLEGRO_TRANSFORM orthographic_transform()
{
    static int a = 0; a += 5; if (a > 6000) a = 0;
    ALLEGRO_BITMAP* bmp = al_get_target_bitmap();
    ALLEGRO_TRANSFORM p;
    float width = al_get_bitmap_width(bmp);
    float height = al_get_bitmap_height(bmp);
    al_identity_transform(&p);
    al_orthographic_transform(&p, 0, 0, 450, width, height, -600);
    return p;
}

static ALLEGRO_TRANSFORM light_view() {
    ALLEGRO_TRANSFORM t;
    Engine::Point vec(1, -3, 3);
    al_build_camera_transform(&t,
        vec.x, vec.y, vec.z,
        0, 0, 0,
        vec.x, vec.y, 0);
    al_translate_transform_3d(&t, 1900, 900, 0);
    return t;
}

static ALLEGRO_TRANSFORM camera_view() {
    ALLEGRO_TRANSFORM t;
    Engine::Point vec(0, 3, 6);
    vec = vec.Normalize();
    vec = vec * 1200;
    al_build_camera_transform(&t,
        vec.x, vec.y, vec.z,
        0, 0, 0,
        0, 1, 0);
    al_translate_transform_3d(&t, -800, -416, 0);
    al_rotate_transform_3d(&t, 1, 0, 0, -ALLEGRO_PI / 24);
    al_use_transform(&t);
    return t;
}
float a = 0;
void DrawCube(std::vector<ALLEGRO_VERTEX> vertices, std::vector<int> indices, ALLEGRO_BITMAP* texture) {
    ALLEGRO_TRANSFORM t;
    al_set_render_state(ALLEGRO_DEPTH_TEST, true);
    ALLEGRO_TRANSFORM defaultTrans;
    al_copy_transform(&defaultTrans, al_get_current_transform());

    Engine::Point mouse = Engine::GameEngine::GetInstance().GetMousePosition();
    
    al_identity_transform(&t);
    al_scale_transform_3d(&t, 50, 50, 50);
    a += 1; if (a > 100) a = 0;
    al_translate_transform_3d(&t, 0, 832, a);
    al_set_shader_matrix("model_matrix", &t);
    al_compose_transform(&t, &defaultTrans);
    al_use_transform(&t);
    al_draw_indexed_prim(vertices.data(), nullptr, texture, indices.data(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);
    
    al_identity_transform(&t);
    al_set_shader_matrix("model_matrix", &t);
    al_use_transform(&defaultTrans);
    al_draw_circle(800, 416, 10, al_map_rgb_f(1, 0, 0), 5);
    al_draw_filled_rectangle(0, 0, 1600, 832, al_map_rgb_f(0, 0, 1));

    al_set_render_state(ALLEGRO_DEPTH_TEST, false);
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
    //al_set_new_bitmap_depth(0);

    texture = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(image.width, image.height), al_destroy_bitmap);
    ALLEGRO_LOCKED_REGION* al_bmp_mem = al_lock_bitmap(texture.get(), ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_WRITEONLY);
    //If pitch is negative
    for (int i = 0; i < image.height; i++) {
        memcpy((uint8_t*)al_bmp_mem->data + i * al_bmp_mem->pitch,
            image.image.data() + i * image.width * 4,
            image.width * al_bmp_mem->pixel_size);
    }
    al_unlock_bitmap(texture.get());

    al_set_new_bitmap_depth(16);
    depthTexture = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(2048, 1536), al_destroy_bitmap);
    render = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(Engine::GameEngine::GetInstance().GetScreenWidth(), Engine::GameEngine::GetInstance().GetScreenHeight()), al_destroy_bitmap);
    al_set_new_bitmap_depth(0);

    //Compiling shaders

    std::string vert =
        "attribute vec4 al_pos;"
        "attribute vec4 al_color;"
        "uniform mat4 view_matrix;"
        "uniform mat4 proj_matrix;"
        "uniform mat4 model_matrix;"
        "void main()"
        "{"
            "gl_Position = proj_matrix * view_matrix * model_matrix * al_pos;"
        "}"
        ;
    std::string frag =
        "void main()"
        "{"
            "gl_FragColor = vec4(gl_FragCoord.zzz, 1.0);"
        "}"
        ;
    shadowShader = std::shared_ptr<ALLEGRO_SHADER>(al_create_shader(ALLEGRO_SHADER_GLSL), al_destroy_shader);
    al_attach_shader_source(shadowShader.get(), ALLEGRO_VERTEX_SHADER, vert.c_str());
    al_attach_shader_source(shadowShader.get(), ALLEGRO_PIXEL_SHADER, frag.c_str());
    //al_attach_shader_source(shadowShader.get(), ALLEGRO_VERTEX_SHADER, al_get_default_shader_source(ALLEGRO_SHADER_GLSL, ALLEGRO_VERTEX_SHADER));
    //al_attach_shader_source(shadowShader.get(), ALLEGRO_PIXEL_SHADER, al_get_default_shader_source(ALLEGRO_SHADER_GLSL, ALLEGRO_PIXEL_SHADER));

    al_build_shader(shadowShader.get());
    if (strlen(al_get_shader_log(shadowShader.get())) > 0) {
        Engine::LOG(Engine::WARN) << al_get_shader_log(shadowShader.get());
    }

    //Load 3d model
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
    //Transformation are per bitmap
    al_set_target_bitmap(depthTexture.get());
    al_clear_depth_buffer(1);
    al_clear_to_color(al_map_rgba_f(1,1,1,1));
    
    //al_set_render_state(ALLEGRO_WRITE_MASK, ALLEGRO_MASK_DEPTH);
    
    al_use_shader(shadowShader.get());
    al_set_shader_matrix("proj_matrix", &orthographic_transform());
    al_set_shader_matrix("view_matrix", &light_view());
    //al_use_projection_transform(&orthographic_transform());
    //al_use_transform(&light_view());
    DrawCube(vertices, indices, texture.get());
    al_use_shader(nullptr);

    al_set_target_bitmap(render.get());
    al_clear_depth_buffer(1);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));

    al_use_projection_transform(&perspective_transform());

    al_use_transform(&camera_view());
    DrawCube(vertices, indices, texture.get());

    al_set_target_backbuffer(al_get_current_display());
    al_draw_bitmap(render.get(), 0, 0, 0);
    al_draw_scaled_bitmap(depthTexture.get(), 0, 0, 2048, 1536, 0, 0, 832, 624, 0);
    //al_draw_scaled_bitmap(depthTexture.get(), 0, 0, 2048, 2048, 1216, 454, 384, 384, 0);
}

void Object3D::Update(float delta)
{
    
}
