#include "Shu.hpp"
#include "Engine/LOG.hpp"

Shu::Shu() :
	Operator("shu/char_2025_shu.skel", "shu/char_2025_shu.atlas", Stats(2313, 442, 553, 10, 0), 22, 70)
{
	tileType = TILE_LOW;
	atkType = TILE_LOW;
	dmgType = Physical;
	stat.SetBlockCount(3);
}

std::string Shu::getIconPath() {
	return "battleSelect/shu.png";
}

void Shu::Update(float delta)
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
		if (curAnim != "Attack_1") {
			state->setAnimation(0, "Attack_1", true);
		}
	}
	else {
		if (curAnim == "Attack_1") {
			state->setAnimation(0, "Idle", true);
		}
	}
}

std::vector<Engine::Point> Shu::getRangeDeltas() const
{
	return { {0, 0}};
}

void Shu::OnAttack()
{
	if (enemiesInRange.empty()) return;
	Enemy* target = enemiesInRange[0];
	AddEffect(new Damage(this, target));
}

void Shu::OnStart()
{
}

void Shu::IsClickedOn()
{
}
