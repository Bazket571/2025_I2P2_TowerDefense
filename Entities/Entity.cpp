#include "Entity.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Collider.hpp"

std::multimap<std::string, Effect*> Entity::effects;

PlayScene* Entity::GetPlayScene()
{
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
}

Engine::Point Entity::GetTile(Engine::Point pos)
{
	Engine::Point ret = pos / (float)PlayScene::BlockSize;
	ret.x = roundf(ret.x);
	ret.y = roundf(ret.y);
	return ret;
}

Entity::Entity(std::string skel, std::string atlas, float x, float y, float z, Stats stat) :
	Engine::SpineSprite(skel, atlas, x, y, 0.4),
	stat(stat)
{
	Position.z = z;
	SpineSprite::state->setAnimation(0, "Default", true);
	SpineSprite::state->setListener(this);
}

Engine::Point Entity::GetCurrentTile() const
{
	return GetTile(Position);
}

void Entity::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event)
{
	if (entry->getAnimation()->getName() == "Start" && type == spine::EventType_Start) {
		OnStart();
	}
	if (entry->getAnimation()->getName() == "Die" && type == spine::EventType_Complete) {
		OnDie();
	}
	if (event == nullptr) return;
	if (event->getData().getName() == "OnAttack") {
		OnAttack();
	}
}

void Entity::Update(float delta)
{
	if (shouldDie) {
		GetPlayScene()->FieldGroup->GetBillboards()->RemoveObject(objectIterator);
		return;
	}
	SpineSprite::Update(delta);
	if (state->getCurrent(0) && state->getCurrent(0)->getAnimation()->getName() == "Die") 
		return;
	else if (stat.GetHP() <= 0) 
		state->setAnimation(0, "Die", false);
	Position = Position + Velocity * delta;
}

void Entity::OnDie()
{
	shouldDie = true;
}

void Entity::IsClickedOn(){}

void Entity::Draw() const
{
	SpineSprite::Draw();
	//Still drawing billboard
	DrawHP();
}

void Entity::DrawHP() const
{
	//Dont draw HP bar when hp is full
	if (stat.GetHP() == stat.GetMaxHP()) return;
	float barSize = PlayScene::BlockSize / 1.5;
	float HpPercent = (float)stat.GetHP() / stat.GetMaxHP();
	ALLEGRO_TRANSFORM trans; al_identity_transform(&trans);
	al_translate_transform_3d(&trans, Position.x - barSize / 2, Position.y + 20, Position.z);
	al_set_shader_matrix("model_matrix", &trans);
	al_draw_filled_rectangle(0, 0, barSize, 5, al_map_rgba_f(0, 0, 0, 0.5));
	al_draw_filled_rectangle(0, 0, barSize * HpPercent, 5, al_map_rgba_f(1, 0, 0, 1));
}

void Entity::OnMouseUp(int button, int mx, int my)
{
	Engine::Point mouse = Billboard::MousePlane({(float)mx, (float)my}, Position.z);
	mouse = GetTile(mouse);
	Engine::Point curPos = GetCurrentTile();

	//Check if the tile we clicked on
	if (mouse == curPos)
		IsClickedOn();
}

void Entity::UpdateEffects()
{
	for(auto iter = effects.begin(); iter != effects.end();){
		auto cur = iter++;
		if (cur->second->duration <= 0) {
			cur->second->after();
			effects.erase(cur);
			continue;
		}
		cur->second->effect();
		cur->second->duration--;
	}
}

void Entity::AddEffect(Effect* effect)
{
	effects.insert({ effect->name, effect });
}

Effect::Effect(std::string name, Entity* from, Entity* to, int duration):
	name(name), from(from), to(to), duration(duration)
{}

void Effect::effect(){

}
void Effect::after() {};

Damage::Damage(Entity* from, Entity* to): Effect("Damage", from, to, 1) {}

void Damage::effect()
{
	Effect::effect();
	if (from->dmgType == Entity::Physical)
		to->stat.SetHP(to->stat.GetHP() - std::max(from->stat.GetAtk() * 0.05, (double)from->stat.GetAtk() - to->stat.GetDef()));
	else if (from->dmgType == Entity::Arts)
		to->stat.SetHP(to->stat.GetHP() - std::max(from->stat.GetAtk() * 0.05, (double)from->stat.GetAtk() * (1 - to->stat.GetRes() / 100.)));
}
Block::Block(Entity* from, Entity* to) :Effect("Block", from, to, 1), prevSpd(0) {}
void Block::effect()
{
	prevSpd = to->stat.GetSpeed();
	to->stat.SetSpeed(0);
}
void Block::after() {
	prevSpd = prevSpd;
}
