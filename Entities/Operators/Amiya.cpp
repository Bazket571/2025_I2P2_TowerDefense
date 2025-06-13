#include "Amiya.hpp"
#include "Engine/LOG.hpp"

Amiya::Amiya(float x, float y, float z, EntityDirection direction) :
	Operator("amiya/char_002_amiya.skel", "amiya/char_002_amiya.atlas", x, y, z, stat, direction)
{
	state->addAnimation(0, "Start", false, 0);
	state->addAnimation(0, "Idle", true, 0);
}

void Amiya::OnAttack()
{
	Engine::LOG(Engine::INFO) << "Amiya attack";
}

void Amiya::Update(float delta)
{
	//Get enemies in range2
	Operator::Update(delta);
	spine::String curAnim = state->getCurrent(0)->getAnimation()->getName();
	if (enemiesInRange.size() > 0) {
		if (curAnim != "Attack" && curAnim != "Attack_Begin") {
			state->setAnimation(0, "Attack_Begin", false);
			state->addAnimation(0, "Attack", true, 0);
		}
	}
	else {
		if (curAnim == "Attack") {
			state->setAnimation(0, "Attack_End", false);
			state->addAnimation(0, "Idle", true, 0);
		}
	}
}

std::vector<Engine::Point> Amiya::getRangeDeltas()
{
	return { {-1, 0}, {0, 0}, {1, 0},
			 {-1, 1}, {0, 1}, {1, 1},
			 {-1, 2}, {0, 2}, {1, 2},
					  {0, 3} };
}

void Amiya::OnStart()
{
	Engine::LOG(Engine::INFO) << "Amiya start";
}

void Amiya::IsClickedOn()
{
	Engine::LOG(Engine::INFO) << "Amiya clicked";
}
