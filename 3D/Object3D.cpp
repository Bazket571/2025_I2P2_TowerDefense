#include "Object3D.hpp"
#include "Engine/Resources.hpp"

Object3D::Object3D(std::string gltfFile, Engine::Point pos, Engine::Point scale, int anchorX, int anchorY)
{
    Position = pos;
    Size = scale;
    auto buffers = Engine::Resources::GetInstance().GetModel(gltfFile, texture);
    vertexBuffer = buffers.first;
    indicesBuffer = buffers.second;
}


void Object3D::Draw() const
{
    //Setup shaders and transforms
    ALLEGRO_TRANSFORM t;
    ALLEGRO_TRANSFORM defaultTrans; //Camera view
    al_copy_transform(&defaultTrans, al_get_current_transform());

    //Setup scales and transforms
    al_identity_transform(&t);
    al_scale_transform_3d(&t, Size.x, Size.y, Size.z);
    al_translate_transform_3d(&t, Position.x, Position.y, Position.z);

    //Setup transforms for shaders and use. They are seperate!!!!
    al_set_shader_matrix("model_matrix", &t);
    al_compose_transform(&t, &defaultTrans);
    //al_use_transform(&t);

    al_draw_circle(0, 0, 1, al_map_rgb_f(0, 1, 0), 2);
    al_draw_indexed_buffer(vertexBuffer.get(), texture.get(), indicesBuffer.get(), 0, al_get_index_buffer_size(indicesBuffer.get()), ALLEGRO_PRIM_TRIANGLE_LIST);
    //Reset transforms used
    //al_use_transform(&defaultTrans);
}

void Object3D::Update(float delta)
{
    
}
