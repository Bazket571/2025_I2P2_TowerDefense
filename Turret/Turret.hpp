#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"

class Enemy;
class PlayScene;

class Turret : public Engine::Sprite {
protected:
    enum TileType {
        LOW_TILE,
        HIGH_TILE
    };
    int price;
    float coolDown;
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;
    TileType tileType;
    int blockCount;
    Sprite imgBase;
    std::list<Turret *>::iterator lockedTurretIterator;
    PlayScene *getPlayScene();
    // Reference: Design Patterns - Factory Method.
    virtual void CreateBullet() = 0;

public:
    bool Enabled = true;
    bool Preview = false;
    Enemy *Target = nullptr;
    Turret(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price, float coolDown, int blockCount);
    void Update(float deltaTime) override;
    void Draw() const override;
    int GetPrice() const;
};
#endif   // TURRET_HPP
