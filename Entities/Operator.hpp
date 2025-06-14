#pragma once
#include <allegro5/allegro.h>

#include <string>
#include <vector>

#include "Entity.hpp"
#include "Enemy.hpp"

#include <Scene/PlayScene.hpp>

class Operator : public Entity {
	std::vector<Engine::Point> RangePreview;
protected:
    virtual std::vector<Engine::Point> getRange();
public:
	std::vector<Enemy*> enemiesInRange;
	bool Preview = true;
	bool Enabled = false;
	mutable EntityDirection direction;
	std::vector<Enemy*> Blocking;
	int cost = 0;
	float redeployTime;
	//Must be inherited
	Operator(std::string skel, std::string atlas, Stats stat, int cost, float redeployTime);
	virtual std::string getIconPath() = 0;

	virtual void Deploy(float x, float y, float z, EntityDirection direction);
	virtual void OnAttack() = 0;
	virtual void OnStart() = 0;
	virtual void Draw() const;
	virtual void Update(float delta);
	virtual void IsClickedOn();
};