#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "Entity.hpp"
#include <Scene/PlayScene.hpp>

class Operator : public Entity {
protected:
    std::string getSkelFile() = 0;
    std::string getAtlasFile() = 0;
    virtual std::vector<Engine::Point> getRange() = 0;
public:
	//Must be inherited
	Operator(float x, float y);

	virtual void OnAttack() = 0;
	virtual void OnStart() = 0;
};