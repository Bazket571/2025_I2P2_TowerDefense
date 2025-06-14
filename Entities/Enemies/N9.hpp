#pragma once

#include "Entities/Enemy.hpp"
#include <vector>

class N9 : public Enemy {
public:
	N9(float x, float y, float z);
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void OnAttack();
};