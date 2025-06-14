#include "N4.hpp"
#include "Engine/LOG.hpp"

N4::N4(float x, float y, float z):
	Enemy("N4/enemy_1014_rogue.skel", "N4/enemy_1014_rogue.atlas", x, y, z, Stats(2000, 130, 100, 5, 1.1))
{
	state->setAnimation(0, "Move", true);
	tileType = TILE_LOW;
}

std::vector<Engine::Point> N4::getRangeDeltas() const
{
	return { 0, 0 };
}

void N4::OnStart()
{
}

void N4::OnAttack()
{
	Engine::LOG(Engine::INFO) << "Slime attacking";
}
