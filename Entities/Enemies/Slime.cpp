#include "Slime.hpp"
#include "Engine/LOG.hpp"

Slime::Slime(float x, float y, float z):
	Enemy("slime/enemy_1007_slime.skel", "slime/enemy_1007_slime.atlas", x, y, z, { 550, 0, 130, 1 })
{

}

std::vector<Engine::Point> Slime::getRangeDeltas()
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
