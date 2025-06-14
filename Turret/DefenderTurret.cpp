#include "DefenderTurret.hpp"

const int DefenderTurret::Price = 50;

DefenderTurret::DefenderTurret(float x, float y, int blockCount):
    Turret("play/tower-base.png", "play/enemy-4.png", x, y, 30, Price, 1, blockCount)
{
    //tileType = TILE_LOW;
}

//This turret will not attack
void DefenderTurret::CreateBullet(){}

void DefenderTurret::Update(float delta)
{
    Sprite::Update(delta);
    PlayScene* scene = getPlayScene();
    imgBase.Position = Position;
    imgBase.Tint = Tint;

    //Check if blocked enemy got killed
    for(int i = 0; i < blocking.size(); i++){
        bool found = false;
       /* for(auto it : scene->EnemyGroup->GetObjects()){
            if(blocking[i] == dynamic_cast<Enemy2*>(it)) {
                found = true; break;
            }
        }*/
        if(!found){
            blocking[i] = blocking.back();
            blocking.pop_back();
            i--;
        }
    }
    if(!Enabled) return;
    //Check for enemy touching
    if(blocking.size() < blockCount){
        //for (auto &it : scene->EnemyGroup->GetObjects()) {
        //    if(blocking.size() >= blockCount) 
        //        break;
        //    bool flag = false;
        //    for(auto enemy : blocking){
        //        if(enemy == it){
        //            flag = true;
        //            break;
        //        }
        //    }
        //    if(flag) continue;
        //    Engine::Point diff = it->Position - Position;
        //    if (diff.Magnitude() <= CollisionRadius) {
        //        Enemy2* e = dynamic_cast<Enemy2 *>(it);
        //        blocking.push_back(e);
        //        e->lockedTurrets.push_back(this);
        //        //lockedTurretIterator = std::prev(Target->lockedTurrets.end());
        //    }
        //}
    }

    //Stop blocked enemies
    for(auto enemy : blocking){
        enemy->speedMultiplier = 0;
    }
}
