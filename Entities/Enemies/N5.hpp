#pragma once

#include "Entities/Enemy.hpp"
#include <vector>

class N5 : public Enemy {
public:
	N5(float x, float y, float z);
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void OnAttack();
};