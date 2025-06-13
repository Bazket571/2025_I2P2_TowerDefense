#include "Billboard.hpp"
#include "Group3D.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include <allegro5/allegro_opengl.h>
#include <algorithm>
#include <Scene/PlayScene.hpp>
std::vector<Engine::IObject*> temp;
static bool CustomCompare(const std::pair<bool, Engine::IObject*>& a, const float y) {
    return a.second->Position.y < y;
}
static bool CustomSort(Engine::IObject* a, Engine::IObject* b) {
    return a->Position.y < b->Position.y;
}
void Billboard::GetModelMatrix(ALLEGRO_TRANSFORM *trans, Engine::Point Position) const {
    al_identity_transform(trans);
    al_translate_transform_3d(trans, Position.x, Position.y, Position.z);
    //al_scale_transform_3d(&trans, it.second->Scale.x, it.second->Scale.y, 1);
}
void Billboard::inverse(ALLEGRO_TRANSFORM* trans) {
    std::vector<float> m;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            m.push_back(trans->m[y][x]);
        }
    }
    float inv[16], det;
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
    addControl(false, dynamic_cast<IControl*>(obj));
}

Billboard::Billboard() : Engine::Group() {
}

void Billboard::Draw() const{
    temp.clear();
    
    //Sort objects by y coords
    //std::sort(objects.begin(), objects.end(), CustomSort);
    ALLEGRO_TRANSFORM trans;
    for (auto& it : objects) {
        temp.push_back(it.second);
    }
    std::sort(temp.begin(), temp.end(), CustomSort);
    for (auto& it : temp) {
        if (it->Visible) {
            GetModelMatrix(&trans, it->Position);
            al_set_shader_matrix("model_matrix", &trans);
            al_set_shader_bool("isBillboard", false);
            al_draw_filled_ellipse(0, 0, PlayScene::BlockSize * it->Scale.x, PlayScene::BlockSize * it->Scale.x / 2, al_map_rgba_f(0,0,0,0.8));
            al_set_shader_bool("isBillboard", true);
            it->Draw();
        }
    }
    //Draw cursor
    Engine::Point mouse = Engine::GameEngine::GetInstance().GetMousePosition();

    GetModelMatrix(&trans, MousePlane(mouse, 0));
    al_set_shader_matrix("model_matrix", &trans);
    al_draw_circle(0, 0, 5, al_map_rgba_f(1, 0, 0, 1), 5);
}

//Check if ray p0-p1 plane at height z
Engine::Point Billboard::RayPlane(Engine::Point p0, Engine::Point p1, float z) {
    Engine::Point nor(0, 0, 1), point(0, 0, z);
    Engine::Point u = p1 - p0;
    float dot = nor.Dot(u);
    Engine::Point w = p0 - point;
    float fac = -(nor.Dot(w)) / dot;
    u = u * fac;
    return p0 + u;
}

Engine::Point Billboard::MousePlane(Engine::Point mouse, float z) {
    Engine::Point p0(mouse.x, mouse.y, -1), p1(mouse.x, mouse.y, 1);
    ScreenToWorld(p0.x, p0.y, p0.z);
    ScreenToWorld(p1.x, p1.y, p1.z);
    return RayPlane(p0, p1, z);
}

//https://stackoverflow.com/questions/7692988/opengl-math-projecting-screen-space-to-world-space-coords
void Billboard::ScreenToWorld(float& x, float& y, float& z)
{
    static ALLEGRO_TRANSFORM invProjView;
    Engine::Point screenSize = Engine::GameEngine::GetInstance().GetScreenSize();
    if (invProjView.m[3][3] == 0) {
        al_identity_transform(&invProjView);
        al_compose_transform(&invProjView, &Group3D::camera_view());
        al_compose_transform(&invProjView, &Group3D::perspective_transform(screenSize.x, screenSize.y));
        inverse(&invProjView);
    }
    static float a = -1; if ((a += 0.025) > 1) a = -1;
    x = x / screenSize.x * 2.f - 1, y = 1.f - y / screenSize.y * 2.f;
    //glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fz);
    //fz = fz * 2 - 1;
    al_transform_coordinates_3d_projective(&invProjView, &x, &y, &z);
}

void Billboard::OnMouseDown(int button, int mx, int my)
{
    float fx = mx, fy = my, fz = 0;
    ScreenToWorld(fx, fy, fz);
    Group::OnMouseDown(button, mx, my);
}

void Billboard::OnMouseUp(int button, int mx, int my)
{
    float fx = mx, fy = my, fz = 0;
    ScreenToWorld(fx, fy, fz);
    Group::OnMouseUp(button, mx, my);
}

void Billboard::OnMouseMove(int mx, int my)
{
    float fx = mx, fy = my, fz = 0;
    ScreenToWorld(fx, fy, fz);
    Group::OnMouseMove(mx, my);
}
