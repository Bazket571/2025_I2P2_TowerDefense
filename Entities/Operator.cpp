#include "Operator.hpp"

void Operator::Update(float delta)
{
    
    SpineSprite::Update(delta);
}

Operator::Operator(float x, float y) : Entity(x, y)
{
}
