#pragma once
#include <string>

#include "Engine/Sprite.hpp"
#include "Enemy/Enemy.hpp"

class Enemy2;
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
    Enemy2* target;
    PlayScene *getPlayScene();
    virtual void OnExplode(Enemy2 *enemy);

public:
    Enemy2 *Target = nullptr;
    explicit HomingBullet(std::string img, float damage, Engine::Point spawnPosition, Enemy2* target);
    void Update(float deltaTime) override;
};