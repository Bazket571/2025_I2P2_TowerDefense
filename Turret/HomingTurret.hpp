#pragma once
#include "Turret.hpp"

class HomingTurret : public Turret {
public:
    static const int Price;
    HomingTurret(float x, float y);
    void CreateBullet();
};
