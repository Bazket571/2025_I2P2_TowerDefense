#include "N7.hpp"
#include "Engine/LOG.hpp"

N7::N7(float x, float y, float z) :
	Enemy("N7/enemy_1015_litamr_2.skel", "N7/enemy_1015_litamr_2.atlas", x, y, z, Stats(3700, 300, 500, 5, 1))
{
	state->setAnimation(0, "Move", true);
	tileType = TILE_LOW;
}

std::vector<Engine::Point> N7::getRangeDeltas() const
{
	return { 0, 0 };
}

void N7::OnStart()
{
}

void N7::OnAttack()
{
}
