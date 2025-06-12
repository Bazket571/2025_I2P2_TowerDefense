#include "Group3D.hpp"

#include <array>

#include "Engine/LOG.hpp"
#include <Engine/GameEngine.hpp>

const std::string Group3D::shadowVert =
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
const std::string Group3D::shadowFrag =
    "#version 330 core\n"
    "void main()"
    "{"
        "gl_FragColor = vec4(gl_FragCoord.zzz, 1.0);"
    "}"
;

const std::string Group3D::mainVert =
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
    "uniform bool isBillboard;"

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
        "if(isBillboard) {"
            "mat4 billboard_matrix = view_matrix * model_matrix;"
            //https://blog.42yeah.is/opengl/rendering/2023/06/24/opengl-billboards.html#solution-2
            "billboard_matrix[0].xyz = vec3(1, 0, 0);"
            "billboard_matrix[1].xyz = vec3(0, 1, 0);"
            "billboard_matrix[2].xyz = vec3(0, 0, 1);"
            "gl_Position = proj_matrix * billboard_matrix * al_pos;"
            "return;"
        "}"
        "gl_Position = proj_matrix * view_matrix * vs_out.FragPos;"
    "}";
const std::string Group3D::mainFrag =
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
    "uniform bool hasShadow;"
    "uniform bool isBillboard;"

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
        "vec4 color = fs_in.Color;"
        "if(al_use_tex) {"
            "color = texture(al_tex, fs_in.TexCoords);"
        "}"
        "if(!hasShadow) {"
            "gl_FragColor = color;"
            "return;"
        "}"

        "float dist = length(lightPos - fs_in.FragPos.xyz);"
        "vec3 normal = normalize(fs_in.Normal);"
        "vec3 lightColor = vec3(1);"
        //Ambient
        "vec3 ambient = 0.2 * lightColor;"
        //Diffuse
        "vec3 lightDir = normalize(lightPos - fs_in.FragPos.xyz);"
        "float diff = max(dot(lightDir, normal), 0);"
        "vec3 diffuse = diff * lightColor;"
        //Specular
        "vec3 viewDir = normalize(viewPos - fs_in.FragPos.xyz);"
        "float spec = 0;"
        "vec3 halfwayDir = normalize(lightDir + viewDir);"
        "spec = pow(max(dot(normal, halfwayDir), 0), shininess);"
        "vec3 specular = spec * lightColor;"
        //calculate shadow
        "float bias = max(0.0025 * (1.0 - dot(normal, lightDir)), 0.005);"
        "float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);"
        "vec3 lighting = (shadow + ambient) * color.rgb;"
        //"vec3 lighting = (ambient + (1 - shadow)) * color;"

        "gl_FragColor = vec4(lighting, color.a);"

    "}";

ALLEGRO_TRANSFORM Group3D::identity = {{ {1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, -1, 0}, {0, 0, 0, 1} }};

ALLEGRO_TRANSFORM Group3D::perspective_transform(float width, float height)
{
    ALLEGRO_TRANSFORM p;
    al_identity_transform(&p);
    al_perspective_transform(&p, -width / 2, -height / 2, width / 2, width / 2, height / 2, 2000);
    al_translate_transform_3d(&p, -800, height, 0);
    al_rotate_transform_3d(&p, 1, 0, 0, -ALLEGRO_PI / 24);
    return p;
}
ALLEGRO_TRANSFORM Group3D::orthographic_transform(float width, float height)
{
    static int a = 0; a += 5; if (a > 6000) a = 0;
    ALLEGRO_TRANSFORM p;
    al_identity_transform(&p);
    al_orthographic_transform(&p, 0, 0, 450, width, height, -600);
    return p;
}
ALLEGRO_TRANSFORM Group3D::light_view() {
    static float a = 0; a += 0.05; if (a > 20) a = 0;
    ALLEGRO_TRANSFORM t;
    Engine::Point vec(1, -3, 5);
    std::array<float, 3> vecArr = { vec.x, vec.y, vec.z };
    al_set_shader_float_vector("lightPos", 3, vecArr.data(), 1);
    al_build_camera_transform(&t,
        vec.x, vec.y, vec.z,
        0, 0, 0,
        vec.x, vec.y, 0);
    al_translate_transform_3d(&t, 1900, 900, 0);
    return t;
}
ALLEGRO_TRANSFORM Group3D::camera_view() {
    static float a = 0; if ((a += 0.25) > 20) a = 0;
    ALLEGRO_TRANSFORM t;
    Engine::Point vec(0, 3, 6);
    vec = vec.Normalize();
    vec = vec * 1200;
    std::array<float, 3> vecArr = { vec.x, vec.y, vec.z };
    al_set_shader_float_vector("viewPos", 3, vecArr.data(), 1);
    al_build_camera_transform(&t,
        vec.x, vec.y, vec.z,
        0, 0, 0,
        0, 1, 0);
    //al_translate_transform_3d(&t, -800, -416, 0);
    //al_rotate_transform_3d(&t, 1, 0, 0, -ALLEGRO_PI / 24);
    return t;
}

Group3D::Group3D() : Group3D::Group3D(false) {};
Group3D::Group3D(bool shadow) : renderShadow(shadow) {
    //Build shaders
    shadowShader = std::shared_ptr<ALLEGRO_SHADER>(al_create_shader(ALLEGRO_SHADER_GLSL), al_destroy_shader);
    if (!attachAndBuildShader(shadowShader.get(), shadowVert, shadowFrag)) {
        Engine::LOG(Engine::WARN) << al_get_shader_log(shadowShader.get());
    }
    mainShader = std::shared_ptr<ALLEGRO_SHADER>(al_create_shader(ALLEGRO_SHADER_GLSL), al_destroy_shader);
    if (!attachAndBuildShader(mainShader.get(), mainVert, mainFrag)) {
        Engine::LOG(Engine::WARN) << al_get_shader_log(mainShader.get());
    }

    //Load render targets
    al_set_new_bitmap_depth(16);
    depthbuffer = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(2400, 1536), al_destroy_bitmap);
    render = std::shared_ptr<ALLEGRO_BITMAP>(al_create_bitmap(Engine::GameEngine::GetInstance().GetScreenWidth(), Engine::GameEngine::GetInstance().GetScreenHeight()), al_destroy_bitmap);
    al_set_new_bitmap_depth(0);

    al_set_target_bitmap(depthbuffer.get());
    al_use_shader(shadowShader.get());
    al_set_target_bitmap(render.get());
    al_use_shader(mainShader.get());
    al_set_target_backbuffer(al_get_current_display());

    billboards = new Billboard();
    AddNewControl(billboards);
};

bool Group3D::attachAndBuildShader(ALLEGRO_SHADER* shader, std::string vertSource, std::string fragSource) {
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

//Manually handle billboard draw calls
void Group3D::Draw() const
{
    //Activate shaders if shadows are needed
    ALLEGRO_TRANSFORM proj, lightProj;
    proj = perspective_transform(al_get_bitmap_width(render.get()), al_get_bitmap_height(render.get()));
    lightProj = orthographic_transform(al_get_bitmap_width(depthbuffer.get()), al_get_bitmap_height(depthbuffer.get()));
    //Depth pass
    if (renderShadow) {
        al_set_target_bitmap(depthbuffer.get());
        al_clear_depth_buffer(1);
        al_clear_to_color(al_map_rgba_f(1, 1, 1, 1));
        //al_use_shader(shadowShader.get());
        al_set_shader_matrix("proj_matrix", &lightProj);
        al_set_shader_matrix("view_matrix", &light_view());

        al_set_render_state(ALLEGRO_DEPTH_TEST, true);
        Group::Draw();
        al_set_render_state(ALLEGRO_DEPTH_TEST, false);
    }
    al_set_target_bitmap(render.get());
    al_clear_depth_buffer(1);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));

    //al_use_shader(mainShader.get());
    al_set_shader_matrix("proj_matrix", &proj);
    al_set_shader_matrix("view_matrix", &camera_view());
    al_set_shader_matrix("light_proj_matrix", &lightProj);
    al_set_shader_matrix("light_view_matrix", &light_view());
    al_set_shader_sampler("depth_map", depthbuffer.get(), 1);
    //Render main stuffs
    al_set_render_state(ALLEGRO_DEPTH_TEST, true);
    al_set_shader_bool("hasShadow", renderShadow);
    al_set_shader_bool("isBillboard", false);
    al_set_render_state(ALLEGRO_DEPTH_FUNCTION, ALLEGRO_RENDER_LESS);
    Group::Draw();
    //Render bill boards
    al_set_shader_bool("hasShadow", false);
    al_set_shader_bool("isBillboard", true);
    al_set_shader_matrix("model_matrix", &identity);
    al_set_render_state(ALLEGRO_DEPTH_FUNCTION, ALLEGRO_RENDER_ALWAYS);
    billboards->Draw();
    al_set_render_state(ALLEGRO_DEPTH_FUNCTION, ALLEGRO_RENDER_LESS);
    al_set_render_state(ALLEGRO_DEPTH_TEST, false);    
    al_set_target_backbuffer(al_get_current_display());
    al_draw_bitmap(render.get(), 0, 0, 0);
    //al_draw_scaled_bitmap(depthbuffer.get(), 0, 0, al_get_bitmap_width(depthbuffer.get()), al_get_bitmap_height(depthbuffer.get()), 0, 0, al_get_bitmap_width(depthbuffer.get()) / 4, al_get_bitmap_height(depthbuffer.get()) / 4, 0);
}

//Manually handle billboard updates
void Group3D::Update(float delta)
{
    billboards->Update(delta);
    Group::Update(delta);
}

void Group3D::AddNewControlBillboard(IObject* obj)
{
    billboards->AddNewControlObject(obj);
}

void Group3D::AddNewBillboard(IObject* obj)
{
    billboards->AddNewObject(obj);
}

Engine::Group* Group3D::GetBillboards()
{
    return billboards;
}
