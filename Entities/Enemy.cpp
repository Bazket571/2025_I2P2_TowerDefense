#include "Enemy.hpp"
#include "Operator.hpp"
#include <random>

Enemy::Enemy(std::string skel, std::string atlas, float x, float y, float z, Stats stat) :
    Entity(skel, atlas, x, y, z, stat), reachEndTime(0)
{
    blockedBy = nullptr;
}

void Enemy::OnStart() {}

void Enemy::OnDie()
{
    if(reachEndTime == 0)
        GetPlayScene()->Hit();
    if (blockedBy) {
        blockedBy->Blocking.erase(this);
        blockedBy = nullptr;
    }
    Entity::OnDie();
}

void Enemy::UpdatePath(const std::vector<std::vector<int>>& mapDistance) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    if (x < 0) x = 0;
    if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;
    Engine::Point pos(x, y);
    int num = mapDistance[y][x];
    if (num == -1) {
        num = 0;
        //Engine::LOG(Engine::ERROR) << "Enemy path finding error";
    }
    path = std::vector<Engine::Point>(num + 1);
    while (num != 0) {
        std::vector<Engine::Point> nextHops;
        for (auto& dir : PlayScene::directions) {
            int x = pos.x + dir.x;
            int y = pos.y + dir.y;
            if (x < 0 || x >= PlayScene::MapWidth || y < 0 || y >= PlayScene::MapHeight || mapDistance[y][x] != num - 1)
                continue;
            nextHops.emplace_back(x, y);
        }
        // Choose arbitrary one.
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
        pos = nextHops[dist(rng)];
        path[num] = pos;
        num--;
    }
    path[0] = PlayScene::EndGridPoint;
}



void Enemy::Update(float delta)
{
    Entity::Update(delta);
    if (shouldDie) return;
    if (state->getCurrent(0)->getAnimation()->getName() == "Die") return;
    Stats curStat = stat;
    //Apply effects on curStat.

    //TODO: Move enemies here
    // Pre-calculate the velocity.
    float remainSpeed = curStat.GetSpeed() * delta * PlayScene::BlockSize / 2;
    if (blockedBy != nullptr) remainSpeed = 0;
    if (remainSpeed == 0) {
        Velocity = { 0,0 };
    }
    while (remainSpeed != 0) {
        if (path.empty()) {
            // Reach end point.
            state->setAnimation(0, "Die", false);
            reachEndTime = 0;
            return;
        }
        Engine::Point target = path.back() * PlayScene::BlockSize;
        Engine::Point vec = target - Position;
        // Add up the distances:
        // 1. to path.back()
        // 2. path.back() to border
        // 3. All intermediate block size
        // 4. to end point
        reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / stat.GetSpeed();
        Engine::Point normalized = vec.Normalize();
        if (remainSpeed - vec.Magnitude() > 0) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        }
        else {
            Velocity = normalized * remainSpeed / delta;
            remainSpeed = 0;
        }
        if (Velocity.x < 0) skeleton->setScaleX(-Scale.x);
        else if (Velocity.x > 0) skeleton->setScaleX(Scale.x);
    }
}
