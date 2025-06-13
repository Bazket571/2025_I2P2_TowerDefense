#include "Operator.hpp"

std::vector<Engine::Point> Operator::getRange()
{
    std::vector<Engine::Point> ret;
    Engine::Point curTile = GetCurrentTile();
    ALLEGRO_TRANSFORM trans; al_identity_transform(&trans);
    al_translate_transform(&trans, curTile.x, curTile.y);
    al_rotate_transform(&trans, direction * ALLEGRO_PI / 2.f);
    for (Engine::Point p : getRangeDeltas()) {
        al_transform_coordinates(&trans, &p.x, &p.y);
        ret.push_back(p);
    }
    return ret;
}

Operator::Operator(std::string skel, std::string atlas, float x, float y, float z, Stats stat, EntityDirection direction) :
    Entity(skel, atlas, x, y, z, stat),
    direction(direction)
{
}

void Operator::Update(float delta)
{
    Entity::Update(delta);
}

void Operator::IsClickedOn(){}