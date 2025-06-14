#pragma once
#include "Entity.hpp"
class Enemy : public Entity {
	std::vector<Engine::Point> path;
public:
	float reachEndTime;
	Operator* blockedBy;

	Enemy(std::string skel, std::string atlas, float x, float y, float z, Stats stat);
	virtual void OnAttack() = 0;
	virtual void OnStart();
	virtual void OnDie();
	void UpdatePath(const std::vector<std::vector<int>>& mapDistance);
	virtual void Update(float delta);
};
