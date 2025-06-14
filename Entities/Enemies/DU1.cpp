#include "DU1.hpp"
#include "Engine/LOG.hpp"

DU1::DU1(float x, float y, float z) :
	Enemy("", "", x, y, z, Stats(2800, 300, 0, 0, 1.7))
{
	state->addAnimation(0, "Move_Begin", false, 0);
	state->addAnimation(0, "Move_Loop", true, 0);
	tileType = TILE_LOW;
}

std::vector<Engine::Point> DU1::getRangeDeltas() const
{
	return { 0, 0 };
}

void DU1::OnStart()
{
}

void DU1::OnAttack()
{
	Engine::LOG(Engine::INFO) << "Slime attacking";
}
