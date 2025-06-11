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
    
    void IScene::Draw() const {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_clear_depth_buffer(1);
        Group::Draw();
        //al_draw_circle(800, 416, 10, al_map_rgb_f(1, 0, 0), 5);
        //DrawCube();
    }
}
