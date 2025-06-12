#include "Entity.hpp"
#include "Engine/GameEngine.hpp"

PlayScene* Entity::GetPlayScene()
{
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
}

Entity::Entity(std::string skel, std::string atlas, float x, float y, int atk, int def, int hp):
	Engine::SpineSprite(skel, atlas, x, y),
	atk(atk), def(def), hp(hp)
{
	//SpineSprite::state->setListener(Listener);
}

void Entity::Listener(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event)
{
	if (event->getStringValue() == "OnAttack") {
		OnAttack();
	}
}

void Entity::ChangeHP(int amount)
{
	hp += amount;
}
