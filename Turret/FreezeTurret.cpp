#include "FreezeTurret.hpp"
#include "Engine/Collider.hpp"
#include "Engine/Resources.hpp"

const int FreezeTurret::Price = 50;

FreezeTurret::FreezeTurret(float x, float y, float freezeRate) :
    Turret("play/tower-base.png", "svgs/regular/snowflake.svg", x, y, 200, Price, 1, 1)
{
    //tileType = TILE_HIGH;
    slowRate = freezeRate;
    Size = Engine::Point(48, 48);
}

//This turret will not attack
void FreezeTurret::CreateBullet() {}

void FreezeTurret::Update(float delta)
{
    Sprite::Update(delta);
    PlayScene* scene = getPlayScene();
    imgBase.Position = Position;
    imgBase.Tint = Tint;

    if (!Enabled) return;

    //Freeze all enemy in range
    /*for (auto &it : scene->EnemyGroup->GetObjects()) {
        Enemy2* e = dynamic_cast<Enemy2*>(it);
        if (Engine::Collider::IsCircleOverlap(e->Position, e->CollisionRadius, Position, CollisionRadius)) {
            e->speedMultiplier *= slowRate;
        }
    }*/
}
