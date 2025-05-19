#pragma once
#include <string>

#include "Engine/Sprite.hpp"
#include "Enemy/Enemy.hpp"

class Enemy;
class PlayScene;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class HomingBullet : public Engine::Sprite {
protected:
    float speed;
    float damage;
    Turret *parent;
    Enemy* target;
    PlayScene *getPlayScene();
    virtual void OnExplode(Enemy *enemy);

public:
    Enemy *Target = nullptr;
    explicit HomingBullet(std::string img, float damage, Engine::Point spawnPosition, Enemy* target);
    void Update(float deltaTime) override;
};