#ifndef BULLET_HPP
#define BULLET_HPP
#include <string>

#include "Engine/Sprite.hpp"

class Enemy2;
class PlayScene;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class Bullet : public Engine::Sprite {
protected:
    float speed;
    float damage;
    Turret *parent;
    PlayScene *getPlayScene();
    virtual void OnExplode(Enemy2 *enemy);

public:
    Enemy2 *Target = nullptr;
    explicit Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void Update(float deltaTime) override;
};
#endif   // BULLET_HPP
