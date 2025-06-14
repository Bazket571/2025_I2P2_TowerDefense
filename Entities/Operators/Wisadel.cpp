#include "Wisadel.hpp"
#include "Engine/LOG.hpp"

Wisadel::Wisadel() :
	Operator("wisadel/char_1035_wisdel.skel", "wisadel/char_1035_wisdel.atlas", Stats(1243, 595, 182, 10, 0), 25, 70)
{
	tileType = TILE_HIGH;
	atkType = TILE_LOW;
	dmgType = Physical;
}

std::string Wisadel::getIconPath() {
	return "battleSelect/wisadel.png";
}

void Wisadel::Update(float delta)
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
		if (curAnim != "Attack_Down_B") {
			state->setAnimation(0, "Attack_Down_B", true);
		}
	}
	else {
		if (curAnim == "Attack_Down_B") {
			state->addAnimation(0, "Idle", true, 0);
		}
	}
}

std::vector<Engine::Point> Wisadel::getRangeDeltas() const
{
	return { {-2, 0}, { -1, 0 }, {0, 0}, {1, 0}, {2,0},
			 {-2, 1}, { -1, 1 }, {0, 1}, {1, 1}, {2,1},
			 {-2, 2}, { -1, 2 }, {0, 2}, {1, 2}, {2,2},
						{-1, 3}, {0, 3}, {1, 3},
								 {0, 4}
	};
}

void Wisadel::OnAttack()
{
	if (enemiesInRange.empty()) return;
	Enemy* target = enemiesInRange[0];
	AddEffect(new Damage(this, target));
}

void Wisadel::OnStart()
{
}

void Wisadel::IsClickedOn()
{
}
