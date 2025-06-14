#include "Logos.hpp"
#include "Engine/LOG.hpp"

Logos::Logos() :
	Operator("logos/char_4133_logos.skel", "logos/char_4133_logos.atlas", Stats(1108, 471, 87, 15, 0), 21, 70)
{
	tileType = TILE_HIGH;
	dmgType = Arts;
}

std::string Logos::getIconPath() {
	return "battleSelect/logos.png";
}

void Logos::Update(float delta)
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
		if (curAnim != "Attack_1" && curAnim != "Attack_2") {
			state->setAnimation(0, "Attack_1", true);
		}
	}
	else {
		if (curAnim == "Attack_1") {
			state->addAnimation(0, "Idle", true, 0);
		}
	}
}

std::vector<Engine::Point> Logos::getRangeDeltas() const
{
	return { {-1, 0}, {0, 0}, {1, 0},
			 {-1, 1}, {0, 1}, {1, 1},
			 {-1, 2}, {0, 2}, {1, 2},
					  {0, 3} };
}

void Logos::OnAttack()
{
	if (enemiesInRange.empty()) return;
	Enemy* target = enemiesInRange[0];
	AddEffect(new Damage(this, target));
}

void Logos::OnStart()
{
	Engine::LOG(Engine::INFO) << "Logos start";
}

void Logos::IsClickedOn()
{
	Engine::LOG(Engine::INFO) << "Logos clicked";
}
