#include "O3.hpp"
#include "Engine/LOG.hpp"

O3::O3(float x, float y, float z) :
	Enemy("O3/enemy_1000_gopro_3.skel", "O3/enemy_1000_gopro_3.atlas", x, y, z, Stats(3000, 370, 0, 0, 1.9))
{
	state->setAnimation(0, "Move", true);
	tileType = TILE_LOW;
}

std::vector<Engine::Point> O3::getRangeDeltas() const
{
	return { 0, 0 };
}

void O3::OnStart()
{
}

void O3::OnAttack()
{
}
