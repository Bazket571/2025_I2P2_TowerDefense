#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "Engine/spine/spine.hpp"
#include <Scene/PlayScene.hpp>

class Entity : public Engine::SpineSprite, Engine::IControl {
    int hp;
    int def;
    int atk;
    //Atk speed will be the time for the animation to run
protected:
    virtual std::vector<Engine::Point> getRangeDeltas() = 0;
    PlayScene* GetPlayScene();
    Engine::Point direction;
    ALLEGRO_TRANSFORM rangeTransform; //For Deltas rotation. Default position will be up.
public:
    Entity(std::string skel, std::string atlas, float x, float y, int atk, int def, int hp);
    void Listener(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event);
    virtual void OnAttack() = 0;
    virtual void OnStart() = 0;
    virtual void ChangeHP(int amount);
};