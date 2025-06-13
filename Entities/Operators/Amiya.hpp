#include "Entities/Operator.hpp"

class Amiya : public Operator {
public:
	Amiya(float x, float y, float z, EntityDirection direction);
	void OnAttack();
	void Update(float delta);

	// Inherited via Entity
	std::vector<Engine::Point> getRangeDeltas() override;
	void OnStart() override;
	void IsClickedOn();
};