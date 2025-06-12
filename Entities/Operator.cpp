#include "Operator.hpp"

Operator::Operator(std::string skel, std::string atlas, float x, float y, float z, Stats stat) :
    Entity(skel, atlas, x, y, z, stat)
{
}

void Operator::Update(float delta)
{
    Entity::Update(delta);
}
