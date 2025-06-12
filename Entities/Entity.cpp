#include "Entity.hpp"
#include "Engine/GameEngine.hpp"

PlayScene* Entity::GetPlayScene()
{
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
}

Entity::Entity(std::string skel, std::string atlas, float x, float y, float z, Stats stat) :
	Engine::SpineSprite(skel, atlas, x, y, 0.4),
	stat(stat)
{
	Position.z = z;
	SpineSprite::state->setListener(this);
}

void Entity::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event)
{
	if (entry->getAnimation()->getName().buffer() == "Start") {
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
}

void Entity::ChangeHP(int amount)
{
	stat.hp += amount;
}
