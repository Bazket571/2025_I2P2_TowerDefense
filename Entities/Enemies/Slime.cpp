#include "Slime.hpp"
#include "Engine/LOG.hpp"

Slime::Slime(float x, float y, float z):
	Enemy("slime/enemy_1007_slime.skel", "slime/enemy_1007_slime.atlas", x, y, z, Stats(550, 130, 0, 0, 1))
{
	state->addAnimation(0, "Move_Begin", false, 0);
	state->addAnimation(0, "Move_Loop", true, 0);
}

std::vector<Engine::Point> Slime::getRangeDeltas() const
{
	return { 0, 0 };
}

void Slime::OnStart()
{
}

void Slime::OnAttack()
{
	Engine::LOG(Engine::INFO) << "Slime attacking";
}
