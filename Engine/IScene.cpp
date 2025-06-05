#include "IScene.hpp"
#include "Engine/GameEngine.hpp"
#include <vector>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

namespace Engine {
    void IScene::Terminate() {
        Clear();
    }
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
    static void set_camera() {
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
    }
    float i = 0;
    float theta = 0;
    void DrawCube(){
        ALLEGRO_STATE state;
        ALLEGRO_TRANSFORM t;

        al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP |
        ALLEGRO_STATE_TRANSFORM |
        ALLEGRO_STATE_PROJECTION_TRANSFORM);
        al_set_render_state(ALLEGRO_DEPTH_TEST, true);
        set_camera();

        i += 0.05;
        theta += 0.05;
        if(i >= 1) i = 0;
        if (theta >= ALLEGRO_PI * 2) theta = 0;
        Engine::Point mouse = Engine::GameEngine::GetInstance().GetMousePosition();
        //al_transform_coordinates(&t, &mouse.x, &mouse.y);  
        al_identity_transform(&t);
        al_rotate_transform(&t, theta);
        al_translate_transform(&t, 800, 416);
        std::vector<Engine::Point> vertexes{ {-50, -50},{-50, 50},{50, -50},{50, 50}};

        std::vector<ALLEGRO_VERTEX> quad2, quad3;
        for (auto p : vertexes) {
            al_transform_coordinates(&t, &p.x, &p.y);
            quad2.push_back({ p.x, p.y, 50, 0, 0, al_map_rgb_f(1, 1, 1) });
            quad3.push_back({ p.x, p.y, 0, 0, 0, al_map_rgba_f(1, 1, 1, 0.5) });
        }
        std::vector<int> indices{0, 1, 2, 2, 3, 1};

        al_draw_circle(0, 0, 20, al_map_rgb_f(0, 1, 0), 5);
        al_draw_rectangle(0, 0, 1600, 832, al_map_rgb_f(0, 0, 1), 10);
        al_draw_circle(800, 416, 10, al_map_rgb_f(1, 0, 0), 5);
        al_draw_indexed_prim(quad2.data(), nullptr, nullptr, indices.data(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);
        al_draw_indexed_prim(quad3.data(), nullptr, nullptr, indices.data(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);
        al_set_render_state(ALLEGRO_DEPTH_TEST, false);
        al_restore_state(&state);
    }
    void IScene::Draw() const {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_clear_depth_buffer(1);
        Group::Draw();
        //al_draw_circle(800, 416, 10, al_map_rgb_f(1, 0, 0), 5);
        //DrawCube();
    }
}
