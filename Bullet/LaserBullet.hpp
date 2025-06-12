#ifndef LASERBULLET_HPP
#define LASERBULLET_HPP
#include "Bullet.hpp"

class Enemy2;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class LaserBullet : public Bullet {
public:
    explicit LaserBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy2 *enemy) override;
};
#endif   // LASERBULLET_HPP
