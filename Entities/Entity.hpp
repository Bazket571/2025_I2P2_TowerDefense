#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>
#include <functional>
#include <map>

#include "Engine/spine/spine.hpp"
#include <Scene/PlayScene.hpp>

enum TileType {
    TILE_LOW = 0b1,
    TILE_HIGH = 0b10,
    TILE_SPAWN = 0b100,
    TILE_OBJECTIVE = 0b1000,
    TILE_BLOCKED = 0b10000, //For roadblocks
    TILE_OCCUPIED_TURRET = 0b100000
};

class Stats {
    int hp = 0;
    int maxHp = 0;
    int atk = 0;
    int def = 0;
    int res = 0;
    float speed = 0;
    int blockCount = 1;
public:
    Stats(int hp, int atk, int def, int res, float speed) :
        hp(hp), maxHp(hp), def(def), atk(atk), res(res), speed(speed) { };

    int GetHP() const { return hp; }; void SetHP(int to) { hp = std::max(0, to); };
    int GetMaxHP() const { return maxHp; }; void SetMaxHP(int to) { maxHp = std::max(0, to); };
    int GetDef() const { return def; }; void SetDef(int to) { def = std::max(0, to); };
    int GetRes() const { return res; }; void SetRes(int to) { res = std::max(0, to); };
    int GetAtk() const { return atk; }; void SetAtk(int to) { atk = std::max(0, to); };
    float GetSpeed() const { return speed; }; void SetSpeed(float to) { speed = std::max(0.f, to); };
    int GetBlockCount() const { return blockCount; } void SetBlockCoung(int to) { blockCount = std::max(0, to); };
};

enum EntityDirection{Down, Right, Up, Left};

class Effect;

class Entity : public Engine::SpineSprite, public Engine::IControl, public spine::AnimationStateListenerObject {
protected:
    //Atk speed will be the time for the animation to run
    Engine::Point Velocity;
    virtual std::vector<Engine::Point> getRangeDeltas() const = 0;
    PlayScene* GetPlayScene();
    bool shouldDie = false;

public:
    Stats stat;
    enum AttackType { Ground = 0b1, Air = 0b10, Both = 0b11 };
    enum DamageType { Physical = 0b1, Arts = 0b10 };
    TileType tileType;
    DamageType dmgType;

    Entity(std::string skel, std::string atlas, float x, float y, float z, Stats stat);
    Engine::Point GetCurrentTile() const;
    static Engine::Point GetTile(Engine::Point pos);
    void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event);
    virtual void Update(float delta);
    virtual void OnDie();
    virtual void OnAttack() = 0;
    virtual void OnStart() = 0;
    virtual void IsClickedOn();
    virtual void Draw() const;
    void DrawHP() const;

    virtual void OnMouseUp(int button, int mx, int my);

    static std::multimap<std::string, Effect*> effects;
    static void UpdateEffects();

    void AddEffect(Effect* effect);

    // Inherited via AnimationStateListenerObject
};

class Effect {
public:
    int duration;
    std::string name;
    Entity* from, *to;

    Effect(std::string name, Entity* from, Entity* to, int duration);
    virtual void effect() = 0;
    virtual void after();
};

class Damage : public Effect {
public:
    Damage(Entity* from, Entity* to);
    void effect();
};

class Block : public Effect {
    float prevSpd;
    Block(Entity* from, Entity* to);
    void effect();
    void after();
};