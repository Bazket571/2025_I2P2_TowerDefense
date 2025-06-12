#include <cmath>
#include "HomingBullet.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
PlayScene *HomingBullet::getPlayScene(){
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

void HomingBullet::OnExplode(Enemy2* enemy){

}

HomingBullet::HomingBullet(std::string img, float damage, Engine::Point spawnPosition, Enemy2 *target) : 
    Sprite(img, spawnPosition.x, spawnPosition.y),
    damage(damage),
    target(target)
{
    speed = 150;
}

void HomingBullet::Update(float delta){
    Sprite::Update(delta);
    Engine::Point direction = (target->Position - Position).Normalize();
    Rotation = atan2(direction.y, direction.x) + ALLEGRO_PI / 2;
    Velocity = direction * speed;
    bool enemyExist = false;
    //Only hit the enemy it aim for
    for (auto &it : getPlayScene()->EnemyGroup->GetObjects()) {
        Enemy2 *enemy = dynamic_cast<Enemy2 *>(it);
        if (!enemy->Visible || enemy != target)
            continue;
        enemyExist = true;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            OnExplode(enemy);
            enemy->Hit(damage);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    //Find new enemy on target death
    if(!enemyExist){
        //no more enemy
        if(getPlayScene()->EnemyGroup->GetObjects().empty()){
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
        target = dynamic_cast<Enemy2*>(getPlayScene()->EnemyGroup->GetObjects().front());
        //OnExplode(nullptr);
        //getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }
}
