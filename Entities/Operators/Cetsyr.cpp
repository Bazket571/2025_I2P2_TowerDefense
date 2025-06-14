#include "Cetsyr.hpp"
#include "Engine/LOG.hpp"

Cetsyr::Cetsyr() :
	//Nerf theresa
	Operator("cetsyr/char_4134_cetsyr.skel", "cetsyr/char_4134_cetsyr.atlas", Stats(1312, 287 / 4, 158, 0, 0), 10, 80)
{
	tileType = TILE_HIGH;
	dmgType = Arts;
	atkSpd = 1.3;
	curTick = 0;
}

std::string Cetsyr::getIconPath() {
	return "battleSelect/cetsyr.png";
}

void Cetsyr::Update(float delta)
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
		curTick += delta;
		if (curTick >= atkSpd) {
			curTick = 0;
			OnAttack();
		}
		if (curAnim != "Skill_1_Begin" && curAnim != "Skill_1_Loop") {
			state->setAnimation(0, "Skill_1_Begin", false);
			state->addAnimation(0, "Skill_1_Loop", true, 0);
		}
	}
	else {
		curTick = 0;
		if (curAnim == "Skill_1_Loop") {
			state->setAnimation(0, "Skill_1_End", false);
			state->addAnimation(0, "Idle", true, 0);
		}
	}
}

std::vector<Engine::Point> Cetsyr::getRangeDeltas() const
{
	return {				  {0,-2},
					 {-1,-1}, {0,-1}, {1,-1},
			{-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2,0},
					 {-1, 1}, {0, 1}, {1, 1},
							  {0, 2}				
	};
}

void Cetsyr::OnAttack()
{
	if (enemiesInRange.empty()) return;
	for(Enemy* e : enemiesInRange)
		AddEffect(new Damage(this, e));
}

void Cetsyr::OnStart()
{
}

void Cetsyr::IsClickedOn()
{
}
