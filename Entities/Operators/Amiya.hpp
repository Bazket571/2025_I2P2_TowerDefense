#include "Entities/Entity.hpp"

class Amiya : public Entity {
	Stats stat;
public:
	Amiya(float x, float y, float z);
	void OnAttack();

	// Inherited via Entity
	std::vector<Engine::Point> getRangeDeltas() override;
	void OnStart() override;
};