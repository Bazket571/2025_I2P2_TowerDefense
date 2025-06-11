#include "Entity.hpp"
#include "Engine/GameEngine.hpp"

PlayScene* Entity::GetPlayScene()
{
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
}

Entity::Entity(std::string skel, std::string atlas, float x, float y):
	Engine::SpineSprite(skel, atlas, x, y)
{
}

void Entity::ChangeHP(int amount)
{
	hp += amount;
}
