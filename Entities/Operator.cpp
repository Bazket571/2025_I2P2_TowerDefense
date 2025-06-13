#include "Operator.hpp"
#include <algorithm>
std::vector<Engine::Point> Operator::getRange()
{
    std::vector<Engine::Point> ret;
    Engine::Point curTile = GetCurrentTile();
    ALLEGRO_TRANSFORM trans; al_identity_transform(&trans);
    al_rotate_transform(&trans, direction * ALLEGRO_PI / 2.f);
    al_translate_transform(&trans, curTile.x, curTile.y);
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
    //Find enemies
    enemiesInRange.clear();
    std::vector<Enemy*> enemies = GetPlayScene()->FieldGroup->GetFromBillboard<Enemy>();
    auto range = getRange();
    for (Enemy* e : enemies) {
        for (Engine::Point p : getRange()) {
            if (p == e->GetCurrentTile() && e->stat.GetHP() > 0) {
                enemiesInRange.push_back(e);
                break;
            }
        }
    }
    //Sort enemies by their distance to objective
    std::sort(enemiesInRange.begin(), enemiesInRange.end(), [](Enemy* a, Enemy* b) {return a->reachEndTime < b->reachEndTime;});
    Entity::Update(delta);
}

void Operator::IsClickedOn(){}