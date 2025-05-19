#include "IScene.hpp"
#include <vector>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

namespace Engine {
    void IScene::Terminate() {
        Clear();
    }
    static void set_perspective_transform(ALLEGRO_BITMAP* bmp)
    {
        ALLEGRO_TRANSFORM p;
        float fov = tan(90 * ALLEGRO_PI / 180 / 2);
        float width = al_get_bitmap_width(bmp);
        float height = al_get_bitmap_height(bmp);
        al_set_target_bitmap(bmp);
        al_identity_transform(&p);
        //al_perspective_transform(&p, -1, aspect_ratio, 1, 1, -aspect_ratio, 1000); //[-1;1]
        al_perspective_transform(&p, -width/2, -height/2, width / 2 / fov, width/2, height/2, 9000);
        al_use_projection_transform(&p);
    }
    float i = 0;
    void DrawCube(){
        ALLEGRO_STATE state;
        ALLEGRO_TRANSFORM projection, t;

        al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP |
        ALLEGRO_STATE_TRANSFORM |
        ALLEGRO_STATE_PROJECTION_TRANSFORM);
        al_set_render_state(ALLEGRO_DEPTH_TEST, true);
        al_build_camera_transform(&t,
            0, 500, 900,
            0, 0, 0,
            0, 1, 0);
        set_perspective_transform(al_get_backbuffer(al_get_current_display()));
        al_use_transform(&t);
        i += 0.05;
        if(i >= 1) i = 0;
        std::vector<ALLEGRO_VERTEX> quad{
            {-500, -200, 0, 0, 0, al_map_rgb_f(1, 0, 0)},
            {-500, 200, 0, 0, 0, al_map_rgb_f(0, 1, 0)},
            {500, -200, 0, 0, 0, al_map_rgb_f(0, 0, 1)},
            {500, 200, 0, 0, 0, al_map_rgb_f(1, 0, 0)}
        };
        std::vector<ALLEGRO_VERTEX> quad2{
            {-100, -100, 30, 0, 0, al_map_rgb_f(1, 1, 1)},
            {-100, 100, 30, 0, 0, al_map_rgb_f(1, 1, 1)},
            {100, -100, 30, 0, 0, al_map_rgb_f(1, 1, 1)},
            {100, 100, 30, 0, 0, al_map_rgb_f(1, 1, 1)}
        };
        std::vector<ALLEGRO_VERTEX> quad3{
            {-100, -100, 0, 0, 0, al_map_rgb_f(i, 1, 1)},
            {-100, 100, 0, 0, 0, al_map_rgb_f(i, 1, 1)},
            {100, -100, 0, 0, 0, al_map_rgb_f(i, 1, 1)},
            {100, 100, 0, 0, 0, al_map_rgb_f(i, 1, 1)}
        };
        std::vector<int> indices{0, 1, 2, 2, 3, 1};

        al_draw_circle(0, 0, 20, al_map_rgb_f(0, 1, 0), 5);
        //al_draw_indexed_prim(quad.data(), nullptr, nullptr, indices.data(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);
        al_draw_indexed_prim(quad2.data(), nullptr, nullptr, indices.data(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);
        al_draw_indexed_prim(quad3.data(), nullptr, nullptr, indices.data(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST);
        al_set_render_state(ALLEGRO_DEPTH_TEST, false);
        al_restore_state(&state);
    }
    void IScene::Draw() const {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_clear_depth_buffer(1);
        Group::Draw();
        //DrawCube();
    }
}
