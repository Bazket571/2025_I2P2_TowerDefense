#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "Entity.hpp"
#include <Scene/PlayScene.hpp>

class Operator : public Entity {
protected:
	Engine::Point direction;
	ALLEGRO_TRANSFORM rangeTransform; //For Deltas rotation. Default position will be down.
    virtual std::vector<Engine::Point> getRange() = 0;
public:
	bool Preview = true;
	enum AttackType {Ground = 0b1, Air = 0b10, Both = 0b11};
	enum DamageType {Physical = 0b1, Arts = 0b10};
	//Must be inherited
	Operator(std::string skel, std::string atlas, float x, float y, float z, Stats stat);

	virtual void OnAttack() = 0;
	virtual void OnStart() = 0;
	virtual void Update(float delta);
};