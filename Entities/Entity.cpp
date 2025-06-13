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
	SpineSprite::state->setListener(this);
}

Engine::Point Entity::GetCurrentTile()
{
	return GetTile(Position);
}

void Entity::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event)
{
	if (entry->getAnimation()->getName() == "Start" && type == spine::EventType_Start) {
		OnStart();
	}
	if (event == nullptr) return;
	if (event->getData().getName() == "OnAttack") {
		OnAttack();
	}
}

void Entity::Update(float delta)
{
	SpineSprite::Update(delta);
	Position = Position + Velocity * delta;
}

void Entity::IsClickedOn(){}

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
		cur->second->effect();
		cur->second->duration--;
		if (cur->second->duration <= 0) {
			effects.erase(cur);
		}

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

Damage::Damage(Entity* from, Entity* to): Effect("Damage", from, to, 1) {}

void Damage::effect()
{
	Effect::effect();
	to->stat.SetHP(to->stat.GetHP() - std::max(5, from->stat.GetAtk() - to->stat.GetDef()));
}
