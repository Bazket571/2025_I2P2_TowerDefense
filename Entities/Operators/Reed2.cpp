#include "Reed2.hpp"
#include "Engine/LOG.hpp"

Reed2::Reed2() :
	Operator("reed2/char_1020_reed2.skel", "reed2/char_1020_reed2.atlas", Stats(1223, 412, 88, 15, 0), 17, 70)
{
	tileType = TILE_HIGH;
	dmgType = Arts;
}

std::string Reed2::getIconPath() {
	return "battleSelect/reed2.png";
}

void Reed2::Update(float delta)
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

std::vector<Engine::Point> Reed2::getRangeDeltas() const
{
	return { {-1, 0}, {0, 0}, {1, 0},
			 {-1, 1}, {0, 1}, {1, 1},
			 {-1, 2}, {0, 2}, {1, 2},
			 {-1, 3}, {0, 3}, {1, 3} 
	};
}

void Reed2::OnAttack()
{
	if (enemiesInRange.empty()) return;
	Enemy* target = enemiesInRange[0];
	AddEffect(new Damage(this, target));
}

void Reed2::OnStart()
{
}

void Reed2::IsClickedOn()
{
}
