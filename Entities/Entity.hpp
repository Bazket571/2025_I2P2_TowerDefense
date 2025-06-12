#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "Engine/spine/spine.hpp"
#include <Scene/PlayScene.hpp>

struct Stats {
    int hp = 0;
    int def = 0;
    int atk = 0;
    float speed = 0;
    Stats(int hp, int def, int atk, float speed) :
        hp(hp), def(def), atk(atk), speed(speed) {};
};
class Effect {
public:
    std::string name;

};
enum EntityDirection{Down, Left, Up, Right};

class Entity : public Engine::SpineSprite, public Engine::IControl, spine::AnimationStateListenerObject {
protected:
    //Atk speed will be the time for the animation to run
    Stats stat;
    Engine::Point Velocity;
    virtual std::vector<Engine::Point> getRangeDeltas() = 0;
    PlayScene* GetPlayScene();
public:
    enum AttackType { Ground = 0b1, Air = 0b10, Both = 0b11 };
    enum DamageType { Physical = 0b1, Arts = 0b10 };
    Entity(std::string skel, std::string atlas, float x, float y, float z, Stats stat);
    void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event);
    virtual void Update(float delta);
    virtual void OnAttack() = 0;
    virtual void OnStart() = 0;
    virtual void ChangeHP(int amount);

    // Inherited via AnimationStateListenerObject
};