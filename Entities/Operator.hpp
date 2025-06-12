#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "Entity.hpp"
#include <Scene/PlayScene.hpp>

class Operator : public Entity {
protected:
    virtual std::vector<Engine::Point> getRange() = 0;
public:
	bool Preview = true;
	//Must be inherited
	Operator(std::string skel, std::string atlas, float x, float y, float z, Stats stat, EntityDirection direction);

	virtual void OnAttack() = 0;
	virtual void OnStart() = 0;
	virtual void Update(float delta);
};