#pragma once

#include "Entities/Enemy.hpp"
#include <vector>

class N4 : public Enemy {
public:
	N4(float x, float y, float z);
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void OnAttack();
};