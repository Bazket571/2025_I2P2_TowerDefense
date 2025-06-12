#pragma once

#include "Entities/Enemy.hpp"
#include <vector>

class Slime : public Enemy {
public:
	Slime(float x, float y, float z);
	std::vector<Engine::Point> getRangeDeltas() override;
	void OnStart() override;
	void OnAttack();
};