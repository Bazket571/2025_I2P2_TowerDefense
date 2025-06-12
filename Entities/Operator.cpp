#include "Operator.hpp"

Operator::Operator(std::string skel, std::string atlas, float x, float y, float z, Stats stat, EntityDirection direction) :
    Entity(skel, atlas, x, y, z, stat)
    //direction(direction)
{
}

void Operator::Update(float delta)
{
    Entity::Update(delta);
}
