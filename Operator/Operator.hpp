#pragma once

#include "Engine/spine/spine.hpp"
#include <string>
#include <vector>
#include <Scene/PlayScene.hpp>

class Operator : public Engine::SpineSprite {
protected:
    virtual std::string getSkelFile() = 0;
    virtual std::string getAtlasFile() = 0;
    virtual std::vector<Engine::Point> getRange() = 0;
	PlayScene* GetPlayScene();
    Engine::Point direction;
public:
	//Must be inherited
	Operator();
    Operator(int gridX, int gridY, float anchorX, float anchorY);

    virtual void Update(float delta);

	virtual void OnAttack() = 0;
	virtual void OnStart() = 0;
};