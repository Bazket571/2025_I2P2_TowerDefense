#pragma once

#include "Entities/Enemy.hpp"
#include <vector>

class O3 : public Enemy {
public:
	O3(float x, float y, float z);
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void OnAttack();
};