#include "N6.hpp"
#include "Engine/LOG.hpp"

N6::N6(float x, float y, float z) :
	Enemy("N6/enemy_1015_litamr.skel", "N6/enemy_1015_litamr.atlas", x, y, z, Stats(2500, 250, 400, 5, 1))
{
	state->setAnimation(0, "Move", true);
	tileType = TILE_LOW;
}

std::vector<Engine::Point> N6::getRangeDeltas() const
{
	return { 0, 0 };
}

void N6::OnStart()
{
}

void N6::OnAttack()
{
}
