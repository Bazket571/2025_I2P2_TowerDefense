#include "Object3D.hpp"

std::vector<ALLEGRO_VERTEX_ELEMENT> Object3D::vertexElems = {
    {ALLEGRO_PRIM_POSITION, ALLEGRO_PRIM_FLOAT_3, offsetof(Object3D::Vertex, x)},
    {ALLEGRO_PRIM_TEX_COORD, ALLEGRO_PRIM_FLOAT_2, offsetof(Object3D::Vertex, u)},
    {ALLEGRO_PRIM_USER_ATTR, ALLEGRO_PRIM_FLOAT_3, offsetof(Object3D::Vertex, nx)}, //Normal
    {ALLEGRO_PRIM_COLOR_ATTR, 0, offsetof(Object3D::Vertex, color)},
    {0,0,0}
};
std::shared_ptr<ALLEGRO_VERTEX_DECL> Object3D::vertexDecl = nullptr;

static ALLEGRO_TRANSFORM perspective_transform(float width, float height)
{
   /* ALLEGRO_BITMAP* bmp = al_get_target_bitmap();
    float width = al_get_bitmap_width(bmp);
    float height = al_get_bitmap_height(bmp);*/
    ALLEGRO_TRANSFORM p;
    al_identity_transform(&p);
    al_perspective_transform(&p, -width / 2, -height / 2, width / 2, width / 2, height / 2, 4000);
    return p;
}
static ALLEGRO_TRANSFORM orthographic_transform(float width, float height)
{
    static int a = 0; a += 5; if (a > 6000) a = 0;
    /*ALLEGRO_BITMAP* bmp = al_get_target_bitmap();
    float width = al_get_bitmap_width(bmp);
    float height = al_get_bitmap_height(bmp);*/
    ALLEGRO_TRANSFORM p;
    al_identity_transform(&p);
    al_orthographic_transform(&p, 0, 0, 450, width, height, -600);
    return p;
}

static ALLEGRO_TRANSFORM light_view() {
    static float a = 0; a += 0.05; if (a > 20) a = 0;
    ALLEGRO_TRANSFORM t;
    Engine::Point vec(1, -3, 5);
    std::array<float, 3> vecArr = { vec.x, vec.y, vec.z};
    al_set_shader_float_vector("lightPos", 3, vecArr.data(), 1);
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
    std::array<float, 3> vecArr = { vec.x, vec.y, vec.z};
    al_set_shader_float_vector("viewPos", 3, vecArr.data(), 1);
    al_build_camera_transform(&t,
        vec.x, vec.y, vec.z,
        0, 0, 0,
        0, 1, 0);
    al_translate_transform_3d(&t, -800, -416, 0);
    al_rotate_transform_3d(&t, 1, 0, 0, -ALLEGRO_PI / 24);
    return t;
}
float a = 0;
void DrawCube(ALLEGRO_VERTEX_BUFFER* vertices, ALLEGRO_INDEX_BUFFER* indices, ALLEGRO_BITMAP* texture) {
    ALLEGRO_TRANSFORM t;
    al_set_render_state(ALLEGRO_DEPTH_TEST, true);
    ALLEGRO_TRANSFORM defaultTrans;
    al_copy_transform(&defaultTrans, al_get_current_transform());

    Engine::Point mouse = Engine::GameEngine::GetInstance().GetMousePosition();
    a += 1; if (a > 100) a = 50;
    
    al_identity_transform(&t);
    al_scale_transform_3d(&t, 50, 50, 50);
    al_translate_transform_3d(&t, 1200, 416, 50);
    al_set_shader_matrix("model_matrix", &t);
    al_draw_indexed_buffer(vertices, texture, indices, 0, al_get_index_buffer_size(indices), ALLEGRO_PRIM_TRIANGLE_LIST);

    al_identity_transform(&t);
    al_scale_transform_3d(&t, 50, 50, 50);
    al_translate_transform_3d(&t, 400, 416, a);
    al_set_shader_matrix("model_matrix", &t);
    al_draw_indexed_buffer(vertices, texture, indices, 0, al_get_index_buffer_size(indices), ALLEGRO_PRIM_TRIANGLE_LIST);
    
    al_identity_transform(&t);
    al_set_shader_matrix("model_matrix", &t);
    al_draw_circle(800, 416, 10, al_map_rgb_f(1, 0, 0), 5);
    al_draw_filled_rectangle(0, 0, 1600, 832, al_map_rgb_f(0, 0, 1));

    al_set_render_state(ALLEGRO_DEPTH_TEST, false);
}

bool attachAndBuildShader(ALLEGRO_SHADER* shader, std::string vertSource, std::string fragSource) {
    if (!al_attach_shader_source(shader, ALLEGRO_VERTEX_SHADER, vertSource.c_str())) {
        return false;
    }
    if (!al_attach_shader_source(shader, ALLEGRO_PIXEL_SHADER, fragSource.c_str())) {
        return false;
    }
    if (!al_build_shader(shader)) {
        return false;
    }
    return true;
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

    //Load texture
    texture = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(image.width, image.height), al_destroy_bitmap);
    ALLEGRO_LOCKED_REGION* al_bmp_mem = al_lock_bitmap(texture.get(), ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_WRITEONLY);
    for (int i = 0; i < image.height; i++) {
        memcpy((uint8_t*)al_bmp_mem->data + i * al_bmp_mem->pitch,
            image.image.data() + i * image.width * 4,
            image.width * al_bmp_mem->pixel_size);
    }
    al_unlock_bitmap(texture.get());

    //Load render targets
    al_set_new_bitmap_depth(16);
    depthbuffer = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(2400, 1536), al_destroy_bitmap);
    render = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(Engine::GameEngine::GetInstance().GetScreenWidth(), Engine::GameEngine::GetInstance().GetScreenHeight()), al_destroy_bitmap);
    al_set_new_bitmap_depth(0);

    //Compiling shaders
    std::string shadowVert =
        "#version 330 core\n"
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
    std::string shadowFrag =
        "#version 330 core\n"
        "void main()"
        "{"
            "gl_FragColor = vec4(gl_FragCoord.zzz, 1.0);"
        "}"
        ;

    std::string mainVert =
        "#version 330 core\n"
        "attribute vec4 al_pos;"
        "attribute vec4 al_color;"
        "attribute vec2 al_texcoord;"
        "attribute vec3 al_user_attr_0;" //Normal

        "uniform mat4 view_matrix;"
        "uniform mat4 proj_matrix;"
        "uniform mat4 model_matrix;"
        "uniform mat4 light_proj_matrix;"
        "uniform mat4 light_view_matrix;"
        "uniform bool al_use_tex_matrix;"
        "uniform mat4 al_tex_matrix;"

        "out VS_OUT {"
            "vec4 FragPos;"
            "vec3 Normal;"
            "vec2 TexCoords;"
            "vec4 FragPosLightSpace;"
            "vec4 Color;"
        "} vs_out;"

        "void main() {"
            "vs_out.FragPos = model_matrix * al_pos;"
            "vs_out.Normal = transpose(inverse(mat3(model_matrix))) * al_user_attr_0;"
            "vs_out.TexCoords = al_texcoord;"
            "if(al_use_tex_matrix) {"
                "vs_out.TexCoords = (al_tex_matrix * vec4(vs_out.TexCoords, 0, 0)).xy;"
            "}"
            "vs_out.FragPosLightSpace = light_proj_matrix * light_view_matrix * vs_out.FragPos;"
            "vs_out.Color = al_color;"
            "gl_Position = proj_matrix * view_matrix * vs_out.FragPos;"
        "}";
    std::string mainFrag = 
        "#version 330 core\n"
        "in VS_OUT {"
            "vec4 FragPos;"
            "vec3 Normal;"
            "vec2 TexCoords;"
            "vec4 FragPosLightSpace;"
            "vec4 Color;"
        "} fs_in;"
        "uniform bool al_use_tex;"
        "uniform sampler2D al_tex;"
        "uniform sampler2D depth_map;"

        "uniform vec3 lightPos;"
        "uniform vec3 viewPos;"

        "float shininess = 4;"

        "float ShadowCalculation(vec4 fragPosLightSpace, float bias) {"
            "vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;" //transform [-w;w] to [-1;1]
            "if(projCoords.z > 1) return 0;"
            "projCoords = projCoords * 0.5 + 0.5;"                           //transform [-1;1] to [0;1]
            "float currentDepth = projCoords.z;"
            "float shadow = 0;"            //check whether current frag pos is in shadow
            "int sampleRadius = 2;"
            "vec2 pixelSize = 1.0 / textureSize(depth_map, 0);"
            "for(int y = -sampleRadius; y <= sampleRadius; y++) {"
                "for(int x = -sampleRadius; x <= sampleRadius; x++) {"
                    "float closestDepth = texture(depth_map, projCoords.xy + vec2(x, y) * pixelSize).r;"      //get closest depth value from light's perspective 
                    "if (currentDepth > closestDepth + bias) shadow += 1;"                        
                "}"
            "}"
            "shadow /= pow((sampleRadius * 2 + 1), 2);"
            "return 1 - shadow;"
        "}"

        "void main() {"
            "vec3 color = fs_in.Color.rgb;"
            "if(al_use_tex) {"
                "color = texture(al_tex, fs_in.TexCoords).rgb;"
            "}"
            "float dist = length(lightPos - fs_in.FragPos.xyz);"
            "vec3 normal = normalize(fs_in.Normal);"
            "vec3 lightColor = vec3(1);"
            //Ambient
            "vec3 ambient = 0.2 * lightColor;"
            //Diffuse
            "vec3 lightDir = -normalize(lightPos - fs_in.FragPos.xyz);"
            "float diff = max(dot(lightDir, normal), 0);"
            "vec3 diffuse = diff * 1.5 * lightColor;"
            //Specular
            "vec3 viewDir = normalize(viewPos - fs_in.FragPos.xyz);"
            "float spec = 0;"
            "vec3 halfwayDir = normalize(lightDir + viewDir);"
            "spec = pow(max(dot(normal, halfwayDir), 0), shininess);"
            "vec3 specular = spec * lightColor;"
            //calculate shadow
            "float bias = max(0.0025 * (1.0 - dot(normal, lightDir)), 0.005);"
            "float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);"
            "vec3 lighting = (shadow * (diffuse + specular) + ambient) * color;"
            //"vec3 lighting = (ambient + (1 - shadow)) * color;"

            "gl_FragColor = vec4(lighting, 1);"

        "}";
    shadowShader = std::shared_ptr<ALLEGRO_SHADER>(al_create_shader(ALLEGRO_SHADER_GLSL), al_destroy_shader);
    if (!attachAndBuildShader(shadowShader.get(), shadowVert, shadowFrag)) {
        Engine::LOG(Engine::WARN) << al_get_shader_log(shadowShader.get());
    }
    mainShader = std::shared_ptr<ALLEGRO_SHADER>(al_create_shader(ALLEGRO_SHADER_GLSL), al_destroy_shader);
    if (!attachAndBuildShader(mainShader.get(), mainVert, mainFrag)) {
        Engine::LOG(Engine::WARN) << al_get_shader_log(mainShader.get());
    }

    //Load 3d model, only accept 1 primitive as we only have a cube
    auto primitive = model.meshes[0].primitives[0];
    std::vector<float> verts = getFromAccessor<float>(primitive.attributes["POSITION"]);
    std::vector<float> normals = getFromAccessor<float>(primitive.attributes["NORMAL"]);
    std::vector<float> uv = getFromAccessor<float>(primitive.attributes["TEXCOORD_0"]);
    std::vector<uint16_t> index = getFromAccessor<uint16_t>(primitive.indices);
    for (int i = 0; i < verts.size() / 3; i++) {
        vertices.push_back({ verts[i * 3 + 0], verts[i * 3 + 1] , verts[i * 3 + 2],
                                uv[i * 2], uv[i * 2 + 1],
                                normals[i * 3 + 0], normals[i*3+1], normals[i*3+2],
                                al_map_rgba_f(1,1,1,1) });
    }
    if (vertexDecl.get() == nullptr) {
        vertexDecl = std::shared_ptr<ALLEGRO_VERTEX_DECL>(al_create_vertex_decl(vertexElems.data(), sizeof(Vertex)), al_destroy_vertex_decl);
    }
    indicesBuffer = std::shared_ptr<ALLEGRO_INDEX_BUFFER>(
        al_create_index_buffer(sizeof(index[0]), index.data(), index.size(), ALLEGRO_PRIM_BUFFER_STATIC), 
        al_destroy_index_buffer);
    vertexBuffer = std::shared_ptr<ALLEGRO_VERTEX_BUFFER>(
        al_create_vertex_buffer(vertexDecl.get(), vertices.data(), vertices.size(), ALLEGRO_PRIM_BUFFER_STATIC), 
        al_destroy_vertex_buffer);
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
    ALLEGRO_TRANSFORM proj, lightProj;
    proj = perspective_transform(al_get_bitmap_width(render.get()), al_get_bitmap_height(render.get()));
    lightProj = orthographic_transform(al_get_bitmap_width(depthbuffer.get()), al_get_bitmap_height(depthbuffer.get()));

    //Transformation are per bitmap
    al_set_target_bitmap(depthbuffer.get());
    al_clear_depth_buffer(1);
    al_clear_to_color(al_map_rgba_f(1,1,1,1));
    
    //al_set_render_state(ALLEGRO_WRITE_MASK, ALLEGRO_MASK_DEPTH);
    
    al_use_shader(shadowShader.get());
    al_set_shader_matrix("proj_matrix", &lightProj);
    al_set_shader_matrix("view_matrix", &light_view());
    //al_use_projection_transform(&orthographic_transform());
    //al_use_transform(&light_view());
    //glCullFace(GL_FRONT);
    DrawCube(vertexBuffer.get(), indicesBuffer.get(), texture.get());
    //glCullFace(GL_BACK);
    al_use_shader(nullptr);

    al_set_target_bitmap(render.get());
    al_clear_depth_buffer(1);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));

    al_use_shader(mainShader.get());
    al_set_shader_matrix("proj_matrix", &proj);
    al_set_shader_matrix("view_matrix", &camera_view());
    al_set_shader_matrix("light_proj_matrix", &lightProj);
    al_set_shader_matrix("light_view_matrix", &light_view());
    al_set_shader_sampler("depth_map", depthbuffer.get(), 1);
    //al_use_projection_transform(&perspective_transform());
    //al_use_transform(&camera_view());

    DrawCube(vertexBuffer.get(), indicesBuffer.get(), texture.get());

    al_use_shader(nullptr);
    al_set_target_backbuffer(al_get_current_display());
    al_draw_bitmap(render.get(), 0, 0, 0);
    //al_draw_scaled_bitmap(depthbuffer.get(), 0, 0, al_get_bitmap_width(depthbuffer.get()), al_get_bitmap_height(depthbuffer.get()), 0, 0, al_get_bitmap_width(depthbuffer.get()) / 4, al_get_bitmap_height(depthbuffer.get()) / 4, 0);
    //al_draw_scaled_bitmap(depthTexture.get(), 0, 0, 2048, 2048, 1216, 454, 384, 384, 0);
}

void Object3D::Update(float delta)
{
    
}
