#include "Turret.hpp"
#include "Enemy/Enemy.hpp"
#include <unordered_set>

class DefenderTurret : public Turret {
    std::vector<Enemy2*> blocking;
    
public:
    static const int Price;
    DefenderTurret(float x, float y, int blockCount);
    void CreateBullet();
    void Update(float delta);
};