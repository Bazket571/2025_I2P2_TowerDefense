#pragma once

#include "Entities/Enemy.hpp"
#include <vector>

class N7 : public Enemy {
public:
	N7(float x, float y, float z);
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void OnAttack();
};