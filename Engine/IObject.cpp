#include "IObject.hpp"
#include "Engine/Point.hpp"
#include "Engine/GameEngine.hpp"

#include <allegro5/allegro_primitives.h>

namespace Engine {
    IObject::IObject(float x, float y, float w, float h, float anchorX, float anchorY) : Position(Point(x, y)), Size(Point(w, h)), Anchor(Point(anchorX, anchorY)) {}
    std::list<std::pair<bool, IObject *>>::iterator IObject::GetObjectIterator() const {
        return objectIterator;
    }
    void IObject::Draw() const {
        if(GameEngine::GetInstance().GetSettings().DrawBoundingBox){
            al_draw_rectangle(Position.x, Position.y, 
                              Position.x + Size.x, Position.y + Size.y, 
                              al_map_rgb_f(0, 1, 0), 1);
        }
    }
    void IObject::Update(float deltaTime) {}
}
