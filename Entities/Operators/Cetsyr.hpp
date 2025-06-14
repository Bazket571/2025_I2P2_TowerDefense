#pragma once
#include "Entities/Operator.hpp"

class Cetsyr : public Operator {
	float atkSpd;
	float curTick;
public:
	Cetsyr();
	void OnAttack();
	void Update(float delta);
	std::string getIconPath();

	// Inherited via Entity
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void IsClickedOn();
};