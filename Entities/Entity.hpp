#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "Engine/spine/spine.hpp"
#include <Scene/PlayScene.hpp>

struct Stats {
    int hp;
    int def;
    int atk;
};
class Effect {
public:
    std::string name;

};

class Entity : public Engine::SpineSprite, public Engine::IControl, spine::AnimationStateListenerObject {
    Stats stat;
    //Atk speed will be the time for the animation to run
protected:
    virtual std::vector<Engine::Point> getRangeDeltas() = 0;
    PlayScene* GetPlayScene();
public:
    Entity(std::string skel, std::string atlas, float x, float y, float z, Stats stat);
    void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event);
    virtual void Update(float delta);
    virtual void OnAttack() = 0;
    virtual void OnStart() = 0;
    virtual void ChangeHP(int amount);

    // Inherited via AnimationStateListenerObject
};