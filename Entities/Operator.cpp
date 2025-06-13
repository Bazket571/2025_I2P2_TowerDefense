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

Operator::Operator(std::string skel, std::string atlas, Stats stat, int cost, float redeployTime) :
    Entity(skel, atlas, 0, 0, 0, stat),
    direction(direction), cost(cost), redeployTime(redeployTime)
{}

void Operator::Deploy(float x, float y, float z, EntityDirection direction)
{
    Position.x = x;
    Position.y = y;
    Position.z = z;
    Preview = false;
    Enabled = true;
    this->direction = direction;
    state->addAnimation(0, "Start", false, 0);
    state->addAnimation(0, "Idle", true, 0);
}

void Operator::Update(float delta)
{
    Entity::Update(delta);
    if (!Enabled) return;
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
}

void Operator::IsClickedOn(){}