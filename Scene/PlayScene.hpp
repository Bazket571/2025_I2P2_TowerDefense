#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP

#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "3D/Group3D.hpp"

class Operator;
class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
};   // namespace Engine


enum TileType {
    TILE_LOW = 0b1,
    TILE_HIGH = 0b10,
    TILE_SPAWN = 0b100,
    TILE_OBJECTIVE = 0b1000,
    TILE_BLOCKED = 0b10000, //For roadblocks
    TILE_OCCUPIED_TURRET = 0b100000
};

class PlayScene final : public Engine::IScene {
private:
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;

protected:
    int lives;
    int money;
    int SpeedMult;

public:
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static int MapWidth, MapHeight;
    static const int MapWidthRatio, MapHeightRatio;
    static const int WindowWidth, WindowHeight;
    static int BlockSize;
    static const float DangerTime;
    static Engine::Point SpawnGridPoint;
    static Engine::Point EndGridPoint;
    static const std::vector<int> code;
    static int score;
    int MapId;
    float ticks;
    float deathCountDown;
    //Operators and their re-deploy timer
    std::vector<std::pair<float, Operator*>> operators;
    // Map tiles.
    Group3D *FieldGroup;
    //Group *TileMapGroup;
    Group *GroundEffectGroup;
    Group *DebugIndicatorGroup;
    //Group *BulletGroup;
    //Group *TowerGroup;
    //Group *EnemyGroup;
    Group *EffectGroup;
    Group *UIGroup;
    Engine::Label *UIMoney;
    Engine::Label *UILives;
    Engine::Label *UIScore;
    Engine::Image *imgTarget;
    //Engine::Sprite *dangerIndicator;
    Turret *preview;
    std::vector<std::vector<int>> mapState;
    std::vector<std::vector<int>> mapDistance;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Hit();
    int GetMoney() const;
    void EarnMoney(int money);
    void AddScore(int point);
    void ReadMap();
    void ReadEnemyWave();
    //void ConstructTurretList();
    void ConstructUI();
    void UIBtnClicked(int id);
    //Return 1: is space valid
    //Return 2: calculated BFS path map if space is valid
    std::pair<bool, std::vector<std::vector<int>>> CheckSpaceValid(int x, int y, TileType type);
    std::vector<std::vector<int>> CalculateBFSDistance();
    void TriggerCheat();
    // void ModifyReadMapTiles();
};
#endif   // PLAYSCENE_HPP
