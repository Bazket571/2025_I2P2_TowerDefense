#include "Amiya.hpp"
#include "Engine/LOG.hpp"

Amiya::Amiya() :
	Operator("amiya/char_002_amiya.skel", "amiya/char_002_amiya.atlas", Stats( 1059, 442, 93, 15, 0), 20, 70)
{
	tileType = TILE_HIGH;
	dmgType = Arts;
}

std::string Amiya::getIconPath() {
	return "battleSelect/amiya.png";
}

void Amiya::Update(float delta)
{
	Operator::Update(delta);
	if (!Enabled) return;
	//Get enemies in range2
	spine::String curAnim = state->getCurrent(0)->getAnimation()->getName();
	//Dont update animation if deploying
	if (curAnim == "Start") return;
	//Dont update animation if dying
	if (curAnim == "Die") return;
	if (enemiesInRange.size() > 0) {
		if (curAnim != "Attack" && curAnim != "Attack_Begin") {
			state->setAnimation(0, "Attack_Begin", false);
			state->addAnimation(0, "Attack", true, 0);
		}
	}
	else {
		if (curAnim == "Attack") {
			state->setAnimation(0, "Attack_End", false);
			state->addAnimation(0, "Idle", true, 0);
		}
	}
}

std::vector<Engine::Point> Amiya::getRangeDeltas() const
{
	return { {-1, 0}, {0, 0}, {1, 0},
			 {-1, 1}, {0, 1}, {1, 1},
			 {-1, 2}, {0, 2}, {1, 2},
					  {0, 3} };
}

void Amiya::OnAttack()
{
	Enemy* target = enemiesInRange[0];
	AddEffect(new Damage(this, target));
	Engine::LOG(Engine::INFO) << "Amiya attack";
}

void Amiya::OnStart()
{
	Engine::LOG(Engine::INFO) << "Amiya start";
}

void Amiya::IsClickedOn()
{
	Engine::LOG(Engine::INFO) << "Amiya clicked";
}
