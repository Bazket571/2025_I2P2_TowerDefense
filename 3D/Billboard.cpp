#include "Billboard.hpp"
#include "Group3D.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include <allegro5/allegro_opengl.h>

static bool CustomCompare(const std::pair<bool, Engine::IObject*>& a, const float y) {
    return a.second->Position.y < y;
}
void Billboard::GetModelMatrix(ALLEGRO_TRANSFORM *trans, Engine::Point Position) const
{
    al_identity_transform(trans);
    al_translate_transform_3d(trans, Position.x, Position.y, Position.z);
    //al_scale_transform_3d(&trans, it.second->Scale.x, it.second->Scale.y, 1);
}
void Billboard::inverse(ALLEGRO_TRANSFORM* trans) const
{
    std::vector<float> m;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            m.push_back(trans->m[y][x]);
        }
    }
    double inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];

    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];

    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        trans->m[i / 4][i % 4] = inv[i] * det;
}
void Billboard::AddNewObject(IObject* obj)
{
    insertObject(true, obj, std::lower_bound(objects.begin(), objects.end(), obj->Position.y, CustomCompare));
}
void Billboard::AddNewControlObject(IObject* obj)
{
    AddNewObject(obj);
    addControl(true, dynamic_cast<IControl*>(obj));
}

void Billboard::Draw() const{
    ALLEGRO_TRANSFORM trans;
    for (auto& it : objects) {
        if (it.second->Visible) {
            GetModelMatrix(&trans, it.second->Position);
            al_set_shader_matrix("model_matrix", &trans);
            it.second->Draw();
        }
    }
    //Draw cursor
    Engine::Point mouse = Engine::GameEngine::GetInstance().GetMousePosition();
    int fx = mouse.x, fy = mouse.y;
    ScreenToWorld(fx, fy);
    GetModelMatrix(&trans, { (float)fx, (float)fy, 0 });
    al_set_shader_matrix("model_matrix", &trans);
    al_draw_circle(0, 0, 5, al_map_rgba_f(1, 0, 0, 1), 5);
}
//https://stackoverflow.com/questions/7692988/opengl-math-projecting-screen-space-to-world-space-coords
void Billboard::ScreenToWorld(int& x, int& y) const
{
    Engine::Point screenSize = Engine::GameEngine::GetInstance().GetScreenSize();
    ALLEGRO_TRANSFORM trans; al_identity_transform(&trans);
    al_compose_transform(&trans, &Group3D::camera_view());
    al_compose_transform(&trans, &Group3D::perspective_transform(screenSize.x, screenSize.y));
    inverse(&trans);
    static float a = -1; if ((a += 0.025) > 1) a = -1;
    float fx = (float)(x) / screenSize.x * 2.f - 1, fy = 1.f - (float)y / screenSize.y * 2.f, fz = 0, fw = 1;
    al_transform_coordinates_4d(&trans, &fx, &fy, &fz, &fw);
    
    x = fx / fw, y = fy / fw;
    
    //Engine::LOG(Engine::INFO) << x << " " << y;
}

void Billboard::OnMouseDown(int button, int mx, int my)
{
    ScreenToWorld(mx, my);
    Group::OnMouseDown(button, mx, my);
}

void Billboard::OnMouseUp(int button, int mx, int my)
{
    ScreenToWorld(mx, my);
    Group::OnMouseUp(button, mx, my);
}

void Billboard::OnMouseMove(int mx, int my)
{
    ScreenToWorld(mx, my);
    Group::OnMouseMove(mx, my);
}
