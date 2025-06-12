#ifndef FIREBULLET_HPP
#define FIREBULLET_HPP
#include "Bullet.hpp"

class Enemy2;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class FireBullet : public Bullet {
public:
    explicit FireBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy2 *enemy) override;
};
#endif   // FIREBULLET_HPP
