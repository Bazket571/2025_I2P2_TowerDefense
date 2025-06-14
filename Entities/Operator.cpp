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
        p.x = roundf(p.x);
        p.y = roundf(p.y);
        ret.push_back(p);
    }
    return ret;
}

Operator::Operator(std::string skel, std::string atlas, Stats stat, int cost, float redeployTime) :
    Entity(skel, atlas, 0, 0, 0, stat),
    direction(Down), cost(cost), redeployTime(redeployTime)
{}

void Operator::Deploy(float x, float y, float z, EntityDirection direction)
{
    Position.x = x;
    Position.y = y;
    Position.z = z;
    Preview = false;
    Enabled = true;
    //this->direction = direction;
    state->addAnimation(0, "Start", false, 0);
    state->addAnimation(0, "Idle", true, 0);
}

void Operator::Draw() const
{
    Entity::Draw();
    //Draw range
    if(Preview) {
        al_set_shader_bool("isBillboard", false);
        for (auto& it : RangePreview) {
            ALLEGRO_TRANSFORM trans;
            al_identity_transform(&trans);
            al_translate_transform_3d(&trans, it.x, it.y, it.z);
            al_set_shader_matrix("model_matrix", &trans);
            al_draw_filled_rectangle(0, 0, PlayScene::BlockSize, PlayScene::BlockSize, al_map_rgba_f(1, 0.3, 0, 0.6));
        }
        al_set_shader_bool("isBillboard", true);
    }
}

void Operator::Update(float delta)
{
    Entity::Update(delta);
    if (Preview) {
        RangePreview.clear();
        for (Engine::Point it : getRange()) {
            PlayScene* playScene = GetPlayScene();
            if (it.y < 0 || it.x < 0 || it.y >= playScene->MapHeight || it.x >= playScene->MapWidth) continue;
            int type = GetPlayScene()->mapState[it.y][it.x];
            float z = (type & TILE_HIGH) ? (PlayScene::BlockSize / 4.f) : 0;
            z += 0.1; //Prevent z fighting
            RangePreview.push_back({ PlayScene::BlockSize * (it.x - 0.5f), PlayScene::BlockSize * (it.y - 0.5f), z });
        }
    }
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