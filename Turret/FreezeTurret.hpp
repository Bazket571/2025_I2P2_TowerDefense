#include "Turret.hpp"
#include "Enemy/Enemy.hpp"
#include <unordered_set>

class FreezeTurret : public Turret {
    float slowRate;

public:
    static const int Price;
    FreezeTurret(float x, float y, float freezeRate);
    void CreateBullet();
    void Update(float delta);
};