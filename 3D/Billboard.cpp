#include "Billboard.hpp"
static bool CustomCompare(const std::pair<bool, Engine::IObject*>& a, const float y) {
    return a.second->Position.y < y;
}
void Billboard::AddNewObject(IObject* obj)
{
    insertObject(true, obj, std::lower_bound(objects.begin(), objects.end(), obj->Position.y, CustomCompare));
}

void Billboard::Draw() const{
    for (auto& it : objects) {
        if (it.second->Visible) {
            ALLEGRO_TRANSFORM trans;
            al_identity_transform(&trans);
            al_translate_transform_3d(&trans, it.second->Position.x, it.second->Position.y, 0);
            //al_scale_transform_3d(&trans, it.second->Scale.x, it.second->Scale.y, 1);
            al_set_shader_matrix("model_matrix", &trans);
            it.second->Draw();
        }
    }
}