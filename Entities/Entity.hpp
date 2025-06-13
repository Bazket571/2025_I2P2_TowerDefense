#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>
#include <functional>
#include <map>

#include "Engine/spine/spine.hpp"
#include <Scene/PlayScene.hpp>

class Stats {
    int hp = 0;
    int maxHp = 0;
    int atk = 0;
    int def = 0;
    float speed = 0;
public:
    Stats(int hp, int atk, int def, float speed) :
        hp(hp), maxHp(hp), def(def), atk(atk), speed(speed) { };

    int GetHP() const { return hp; };
    void SetHP(int to) { hp = std::max(0, to); };
    int GetMaxHP() const { return maxHp; };
    void SetMaxHP(int to) { maxHp = std::max(0, to); };
    int GetDef() const { return def; }
    void SetDef(int to) { def = std::max(0, to); };
    int GetAtk() const { return atk; }
    void SetAtk(int to) { atk = std::max(0, to); };
    float GetSpeed() const { return speed; }
    void SetSpeed(float to) { speed = std::max(0.f, to); };
};

enum EntityDirection{Down, Left, Up, Right};

class Effect;

class Entity : public Engine::SpineSprite, public Engine::IControl, public spine::AnimationStateListenerObject {
protected:
    //Atk speed will be the time for the animation to run
    Engine::Point Velocity;
    virtual std::vector<Engine::Point> getRangeDeltas() = 0;
    PlayScene* GetPlayScene();
    Engine::Point GetTile(Engine::Point pos);

public:
    Stats stat;
    enum AttackType { Ground = 0b1, Air = 0b10, Both = 0b11 };
    enum DamageType { Physical = 0b1, Arts = 0b10 };
    Entity(std::string skel, std::string atlas, float x, float y, float z, Stats stat);
    Engine::Point GetCurrentTile();
    void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event);
    virtual void Update(float delta);
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
};

class Damage : public Effect {
public:
    Damage(Entity* from, Entity* to);
    void effect();
};