#pragma once
#include "Engine/Group.hpp"

#include "allegro5/allegro.h"
#include <memory>
class Billboard : public Engine::Group{
public:
	void AddNewObject(IObject* obj);
	void Draw() const override;
};