#pragma once

#include "Entities/Enemy.hpp"
#include <vector>

class DU1 : public Enemy {
public:
	DU1(float x, float y, float z);
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void OnAttack();
};