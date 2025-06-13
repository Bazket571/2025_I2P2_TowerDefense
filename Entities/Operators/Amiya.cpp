#include "Amiya.hpp"
#include "Engine/LOG.hpp"

Amiya::Amiya(float x, float y, float z) :
	Operator("amiya/char_002_amiya.skel", "amiya/char_002_amiya.atlas", x, y, z, stat, Down)
{
	state->setAnimation(0, "Start", false);
	state->addAnimation(0, "Idle", true, 0);
}

void Amiya::OnAttack()
{
	Engine::LOG(Engine::INFO) << "Amiya attack";
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
