#include "N9.hpp"
#include "Engine/LOG.hpp"

N9::N9(float x, float y, float z):
	Enemy("N9/enemy_1006_shield_2.skel", "N9/enemy_1006_shield_2.atlas", x, y, z, Stats(10000, 600, 1000, 20, 0.75))
{
	state->addAnimation(0, "Move_Begin", false, 0);
	state->addAnimation(0, "Move_Loop", true, 0);
	tileType = TILE_LOW;
}

std::vector<Engine::Point> N9::getRangeDeltas() const
{
	return { 0, 0 };
}

void N9::OnStart()
{
}

void N9::OnAttack(){

}
