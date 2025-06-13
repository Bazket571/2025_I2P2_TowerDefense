#include "Entities/Operator.hpp"

class Amiya : public Operator {
	Stats stat{ 699, 276, 48, 0 };
public:
	Amiya(float x, float y, float z);
	void OnAttack();

	// Inherited via Entity
	std::vector<Engine::Point> getRangeDeltas() override;
	void OnStart() override;
	void IsClickedOn();
};