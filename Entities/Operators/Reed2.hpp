#pragma once
#include "Entities/Operator.hpp"

class Reed2 : public Operator {
public:
	Reed2();
	void OnAttack();
	void Update(float delta);
	std::string getIconPath();

	// Inherited via Entity
	std::vector<Engine::Point> getRangeDeltas() const override;
	void OnStart() override;
	void IsClickedOn();
};