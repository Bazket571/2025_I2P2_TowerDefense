#include "HomingTurret.hpp"
#include "Engine/AudioHelper.hpp"
#include "Bullet/HomingBullet.hpp"
#include "Scene/PlayScene.hpp"

const int HomingTurret::Price = 100;
HomingTurret::HomingTurret(float x, float y):
    Turret("play/tower-base.png", "play/turret-3.png", x, y, 200, Price, 1, 1)
{
    tileType = TILE_HIGH;
}

void HomingTurret::CreateBullet(){
    getPlayScene()->BulletGroup->AddNewObject(new HomingBullet("play/bullet-3.png", 60, Position, Target));
    AudioHelper::PlayAudio("gun.wav");
}