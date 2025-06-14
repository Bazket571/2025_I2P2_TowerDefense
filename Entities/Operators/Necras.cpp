#include "Necras.hpp"
#include "Engine/LOG.hpp"

Necrass::Necrass() :
	Operator("necrass/char_450_necras.skel", "necrass/char_450_necras.atlas", Stats(1443, 576, 155, 20, 0), 21, 70)
{
	tileType = TILE_HIGH;
	atkType = TILE_LOW | TILE_HIGH;
	dmgType = Arts;
}

std::string Necrass::getIconPath() {
	return "battleSelect/necrass.png";
}

void Necrass::Update(float delta)
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
		if (curAnim != "Attack") {
			state->setAnimation(0, "Attack", true);
		}
	}
	else {
		if (curAnim == "Attack") {
			state->addAnimation(0, "Idle", true, 0);
		}
	}
}

std::vector<Engine::Point> Necrass::getRangeDeltas() const
{
	return { {-1, 0}, {0, 0}, {1, 0},
			 {-1, 1}, {0, 1}, {1, 1},
			 {-1, 2}, {0, 2}, {1, 2}
					, {0, 3} };
}

void Necrass::OnAttack()
{
	if (enemiesInRange.empty()) return;
	Enemy* target = enemiesInRange[0];
	AddEffect(new Damage(this, target));
}

void Necrass::OnStart()
{
}

void Necrass::IsClickedOn()
{
}
