#pragma once
#include "Entities/Operator.hpp"

class Amiya : public Operator {
public:
	Amiya();
	void OnAttack();
	void Update(float delta);
	std::string getIconPath();

	// Inherited via Entity
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void IsClickedOn();
};