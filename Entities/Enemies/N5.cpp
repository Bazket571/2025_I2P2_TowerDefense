#include "N5.hpp"
#include "Engine/LOG.hpp"

N5::N5(float x, float y, float z) :
	Enemy("N5/enemy_1014_rogue_2.skel", "N5/enemy_1014_rogue_2.atlas", x, y, z, Stats(3000, 450, 120, 5, 1.1))
{
	state->setAnimation(0, "Move", true);
	tileType = TILE_LOW;
}

std::vector<Engine::Point> N5::getRangeDeltas() const
{
	return { 0, 0 };
}

void N5::OnStart()
{
}

void N5::OnAttack()
{
}
