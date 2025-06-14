//#define ALLEGRO_UNSTABLE

#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include <array>
#include <sstream>

#include "Entities/Enemies.hpp"
#include "Enemy/Enemy.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "3D/Object3D.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/HomingTurret.hpp"
#include "Turret/TurretButton.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/Plane.hpp"
#include "UI/Component/Label.hpp"
#include <Turret/DefenderTurret.hpp>
#include <Turret/FreezeTurret.hpp>
#include <Engine/spine/spine.hpp>
#include "Entities/Operators.hpp"
#include "UI/Video.hpp"
#include "Engine/Collider.hpp"

// TODO HACKATHON-4 (1/3): Trace how the game handles keyboard input.
// TODO HACKATHON-4 (2/3): Find the cheat code sequence in this file.
// TODO HACKATHON-4 (3/3): When the cheat code is entered, a plane should be spawned and added to the scene.
// TODO HACKATHON-5 (1/4): There's a bug in this file, which crashes the game when you win. Try to find it.
// TODO HACKATHON-5 (2/4): The "LIFE" label are not updated when you lose a life. Try to fix it.

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidthRatio = 5, PlayScene::MapHeightRatio = 3;
int PlayScene::MapWidth = 0, PlayScene::MapHeight = 0;
int PlayScene::BlockSize = 96;
int PlayScene::score = 0;
const int PlayScene::WindowWidth = (64*21), PlayScene::WindowHeight = 64*13;
std::vector<Engine::Point> PlayScene::SpawnGridPoint = {};
Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
int PlayScene::OperatorUISize = 96;

Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
    mapState.clear();
    keyStrokes.clear();
    enemyWaveData.clear();
    ticks.clear();
    deathCountDown = -1;
    lives = 3;
    DP = 10;
    DPRegenRate = 1;
    DPTick = 0;
    SpeedMult = 1;
    score = 0;
    // Add groups from bottom to top.
    //TileMapGroup = new Group();
    UIGroup = new Group();
    //AddNewObject();
    //AddNewObject(new Video("Resource/test.ogv", { 0, 0 }, { 1600, 832 }));
    AddNewControlObject(FieldGroup = new Group3D(true));
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    // Should support buttons.
    AddNewControlObject(UIGroup = new Group());
    AddNewControlObject(OperatorButtons = new Group());
    AddNewControlObject(OperatorButtonsFrames = new Group());
    operators.clear();
    operators.push_back({ 0, new Amiya() });
    operators.push_back({ 0, new Logos() });
    operators.push_back({ 0, new Necrass() });
    operators.push_back({ 0, new Reed2() });
    operators.push_back({ 0, new Wisadel() });
    operators.push_back({ 0, new Cetsyr() });
    operators.push_back({ 0, new Shu() });

    std::sort(operators.begin(), operators.end(), 
        [](std::pair<float, Operator*> a, std::pair<float, Operator*> b) {return a.second->cost > b.second->cost;});
    //FieldGroup->AddNewControlBillboard(amiyi); //Low ground offset
    //amiyi->Deploy(5 * BlockSize, 2 * BlockSize, 0, Right);
    //FieldGroup->AddNewControlBillboard(new Amiya(5 * BlockSize, (2) * BlockSize, 0, Right));        //High ground
    ReadMap();
    for(int i = 0; i < SpawnCount; i++)
        ReadEnemyWave(i);
    mapDistance = CalculateBFSDistance();
    ConstructUI();
    //imgTarget = new Engine::Image("play/target.png", 0, 0);
    //imgTarget->Visible = false;
    preview = nullptr;
    //UIGroup->AddNewObject(imgTarget);
    mouseDownPos = { -1, -1 };

    // Start BGM.
    bgmId = AudioHelper::PlayBGM("play.ogg");
}
void PlayScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {
    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;
    // Calculate danger zone.
    // Reset all enemy's speed multiplier
    std::vector<float> reachEndTimes;
    /*for (auto &it : EnemyGroup->GetObjects()) {
        dynamic_cast<Enemy2 *>(it)->speedMultiplier = 1;
        reachEndTimes.push_back(dynamic_cast<Enemy2 *>(it)->reachEndTime);
    }*/
    // Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
    std::sort(reachEndTimes.begin(), reachEndTimes.end());
    float newDeathCountDown = -1;
    int danger = lives;
    deathCountDown = newDeathCountDown;
    if (SpeedMult == 0)
        deathCountDown = -1;
    for (int i = 0; i < SpeedMult; i++) {
        IScene::Update(deltaTime);
        Entity::UpdateEffects();
        UpdateEnemyWave(deltaTime);
        //Update DP
        if(GetDP() <= 99)
            DPTick += deltaTime;
        if (DPTick >= DPRegenRate) {
            DPTick = 0;
            EarnDP(1);
        }
    }
    if (preview) {
        if (mouseDownPos == Engine::Point(-1, -1, 0))
            preview->Position = Entity::GetTile(Billboard::MousePlane(Engine::GameEngine::GetInstance().GetMousePosition(), 0)) * BlockSize;
        // To keep responding when paused.
        preview->Update(deltaTime);
    }
    UpdateOperatorUI();
}
void PlayScene::Draw() const {
    IScene::Draw();
    if (DebugMode) {
        // Draw reverse BFS distance on all reachable blocks.
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if (mapDistance[i][j] != -1) {
                    // Not elegant nor efficient, but it's quite enough for debugging.
                    Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
                    label.Anchor = Engine::Point(0.5, 0.5);
                    label.Draw();
                }
            }
        }
    }
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
    const Engine::Point mouseDownTile = Entity::GetTile(Billboard::MousePlane({ (float)mx, (float)my }, 0));
    if ((button & 1) && preview) {
        if (Engine::Collider::IsPointInRect(mouseDownTile,{ 0.f, 0.f}, { MapWidth - 1.f,MapHeight - 1.f }))
        if (!(mapState[mouseDownTile.y][mouseDownTile.x] & (TILE_OCCUPIED_TURRET | TILE_FORBIDDEN)))
        if (mapState[mouseDownTile.y][mouseDownTile.x] & preview->tileType)
        if (mouseDownPos == Engine::Point(-1, -1, 0)) {
            mouseDownPos.x = mx;
            mouseDownPos.y = my;
        }
    }
    IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
    if (mouseDownPos != Engine::Point(-1, -1) && preview) {
        //Get vector angle to the negative y axis
        Engine::Point v = mouseDownPos - Engine::Point(mx, my);
        //Engine::LOG(Engine::INFO) << v.x << " " << v.y;
        if (abs(v.x) > abs(v.y)) {
            preview->direction = (v.x > 0) ? Right : Left;
        }
        else {
            preview->direction = (v.y > 0) ? Up : Down;
        }
    }
    IScene::OnMouseMove(mx, my);
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    /*if (!imgTarget->Visible)
        return;*/
    if (button & 1) {
        const Engine::Point mouseDownTile = Entity::GetTile(Billboard::MousePlane(mouseDownPos, 0));
        if (mouseDownTile.x >= MapWidth || mouseDownTile.y >= MapHeight || mouseDownTile.x < 0 || mouseDownTile.y < 0) {
            return;
        }
        //Check if allowed
        if (!(mapState[mouseDownTile.y][mouseDownTile.x] & (TILE_OCCUPIED_TURRET|TILE_FORBIDDEN))) {
            if (!preview)
                return;
            // Check if valid.
            auto result = CheckSpaceValid(mouseDownTile.x, mouseDownTile.y, TILE_OCCUPIED_TURRET);
            if (!result.first || !(mapState[mouseDownTile.y][mouseDownTile.x] & preview->tileType)) {
                Engine::Sprite *sprite;
                FieldGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, mouseDownTile.x * BlockSize, mouseDownTile.y * BlockSize));
                sprite->Rotation = 0;
                return;
            }
            // Purchase.
            EarnDP(-preview->cost);
            //Deploy preview
            Engine::Point deployPos = mouseDownTile * BlockSize;
            preview->Deploy(deployPos.x, deployPos.y, deployPos.z, preview->direction);
            // Remove Preview.
            preview = nullptr;
            
            mapState[mouseDownTile.y][mouseDownTile.x] |= TILE_OCCUPIED_TURRET;
            mapDistance = result.second;
            SpeedMult = PrevSpeedMult;
            operators[curSelectIndex].first = -1;
            curSelectIndex = -1;
            mouseDownPos = { -1, -1, 0 };
            OnMouseMove(mx, my);
        }
    }
}
void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    }
    if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        // Hotkey for Speed up.
        SpeedMult = keyCode - ALLEGRO_KEY_0;
    }
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        if (preview) {
            FieldGroup->GetBillboards()->RemoveControlObject(preview->GetControlIterator(), preview->GetObjectIterator());
            preview = nullptr;
            SpeedMult = PrevSpeedMult;
            curSelectIndex = -1;
            mouseDownPos = { -1, -1, 0 };
        }
        else SpeedMult = 0;
    }
}
void PlayScene::Hit() {
    lives--;
    //UILives->Text = "Life " + std::to_string(lives);
    if (lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("lose");
    }
}
int PlayScene::GetDP() const {
    return DP;
}
void PlayScene::EarnDP(int DP) {
    this->DP += DP;
    UIDP->Text = std::to_string(this->DP);
}
void PlayScene::AddScore(int point)
{
    this->score += point;
    UIScore->Text = std::string("Score ") + std::to_string(this->score);
}
void PlayScene::ReadMap()
{
    SpawnCount = 0;
    std::string filename = std::string("Resource/map") + MapId + ".txt";
    // Read map file.
    //TODO MapData has 2 states only, change this
    std::vector<int> mapData;
    std::list<int> spawnIndexOrder;
    std::ifstream fin(filename);
    MapWidth = MapHeight = 0;
    while (1) {
        std::string line;
        std::getline(fin, line);
        if (line.length() == 0) break;
        for(char c : line) {
            if (isdigit(c)) {
                mapData.push_back(TILE_SPAWN);
                spawnIndexOrder.push_back(c - '0');
                SpawnCount++;
                continue;
            }
            switch (c) {
            case 'l': mapData.push_back(TILE_LOW); break;
            case 'h': mapData.push_back(TILE_HIGH); break;
            case 'L': mapData.push_back(TILE_LOW | TILE_FORBIDDEN); break;
            case 'H': mapData.push_back(TILE_HIGH | TILE_FORBIDDEN); break;
            case 'o': mapData.push_back(TILE_OBJECTIVE); break;

            case '\r':
            case '\n':
                break;
            default: throw std::ios_base::failure("Map data is corrupted.");
            }
        }
        MapHeight++;
        if (MapWidth == 0)
            MapWidth = static_cast<int>(mapData.size());
        //Validate map is a rectangle
        else if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
            throw std::ios_base::failure("Map data is not rectangular.");;
    }
    fin.close();
    SpawnGridPoint.resize(SpawnCount);
    ticks.resize(SpawnCount, 0);
    enemyWaveData.resize(SpawnCount);
    // Store map in 2d array.
    // Test Change buffer depth
    //al_set_new_bitmap_depth(16);
    mapState = std::vector<std::vector<int>>(MapHeight, std::vector<int>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            mapState[i][j] = mapData[i * MapWidth + j];
            Engine::Point scale = { (float)BlockSize/2, (float)BlockSize/2, 1 };
            //TODO Map has only 2 states, change this
            switch(mapState[i][j]){
                case TILE_LOW:
                    //TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/lground black.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)-BlockSize / 2 }, scale));
                    break;
                case TILE_HIGH:
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/hground white.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)-BlockSize / 4 }, scale));
                    break;
                case TILE_LOW | TILE_FORBIDDEN:
                    //TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/lground white.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)-BlockSize / 2 }, scale));
                    break;
                case TILE_HIGH | TILE_FORBIDDEN:
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/hground black.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)-BlockSize / 4 }, scale));
                    break;
                case TILE_SPAWN:
                    SpawnGridPoint[spawnIndexOrder.front()] = Engine::Point(j, i);
                    spawnIndexOrder.pop_front();
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/TileLow.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)-BlockSize / 2 }, scale));
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/RedBox.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)BlockSize / 2 }, scale));
                    break;
                case TILE_OBJECTIVE:
                    EndGridPoint = Engine::Point(j, i);
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/TileLow.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)-BlockSize / 2 }, scale));
                    FieldGroup->AddNewObject(new Object3D("Resource/3D/BlueBox.glb", { (float)j * BlockSize, (float)i * BlockSize, (float)BlockSize / 2 }, scale));
                    break;
                default:
                    //TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
                    break;
            }
        }
    }
    //al_set_new_bitmap_depth(0);
    //Change blockSize according to aspect ratio
}
void PlayScene::ReadEnemyWave(int spawnNo) {
    std::string filename = std::string("Resource/enemy") + MapId + "$" + std::to_string(spawnNo) + ".txt";
    // Read enemy file.
    std::string type;
    float wait, repeat;
    enemyWaveData[spawnNo].clear();
    std::ifstream fin(filename);
    while (fin >> type && fin >> wait && fin >> repeat) {
        for (int i = 0; i < repeat; i++)
            enemyWaveData[spawnNo].emplace_back(type, wait);
    }
    fin.close();
}
void PlayScene::UpdateEnemyWave(float deltaTime)
{
    bool ShouldWin = true;
    // Check if we should create new enemy.
    for (int i = 0; i < SpawnCount; i++) {
        if (!enemyWaveData[i].empty()) {
            ShouldWin = false; break;
        }
    }
    if (!FieldGroup->GetFromBillboard<Enemy>().empty()) {
        ShouldWin = false;
    }
    if(ShouldWin) Engine::GameEngine::GetInstance().ChangeScene("win");
    for (int i = 0; i < SpawnCount; i++) {
        ticks[i] += deltaTime;
        if (enemyWaveData[i].empty()) continue;
        auto current = enemyWaveData[i].front();
        if (ticks[i] < current.second)
            continue;
        ticks[i] -= current.second;
        enemyWaveData[i].pop_front();
        const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint[i].x * BlockSize, SpawnGridPoint[i].y * BlockSize);
        Enemy* enemy = nullptr;
        //TODO Add enemy here
        if (current.first == "B1") {
            FieldGroup->AddNewBillboard(enemy = new Slime(SpawnCoordinate.x, SpawnCoordinate.y, 0));
        }
        else if (current.first == "O3") {
            FieldGroup->AddNewBillboard(enemy = new O3(SpawnCoordinate.x, SpawnCoordinate.y, 0));
        }
        else if (current.first == "N4") {
            FieldGroup->AddNewBillboard(enemy = new N4(SpawnCoordinate.x, SpawnCoordinate.y, 0));
        }
        else if (current.first == "N5") {
            FieldGroup->AddNewBillboard(enemy = new N5(SpawnCoordinate.x, SpawnCoordinate.y, 0));
        }
        else if (current.first == "N6") {
            FieldGroup->AddNewBillboard(enemy = new N6(SpawnCoordinate.x, SpawnCoordinate.y, 0));
        }
        else if (current.first == "N7") {
            FieldGroup->AddNewBillboard(enemy = new N7(SpawnCoordinate.x, SpawnCoordinate.y, 0));
        }
        else if (current.first == "N9") {
            FieldGroup->AddNewBillboard(enemy = new N9(SpawnCoordinate.x, SpawnCoordinate.y, 0));
        }

        if (enemy != nullptr) {
            enemy->UpdatePath(mapDistance);
            // Compensate the time lost.
            enemy->Update(ticks[i]);
        }
    }
}

void PlayScene::UpdateOperatorUI() {
    Engine::Point screenSize = Engine::GameEngine::GetInstance().GetScreenSize();
    int opNo = 0;
    int i = -1;
    for (auto &button : OperatorButtons->GetObjects()) {
        i++;
        if (operators[i].first == -1 || operators[i].second->cost > GetDP()) {
            button->Visible = false;
            dynamic_cast<Engine::ImageButton*>(button)->Enabled = false;
            continue;
        }
        button->Visible = true;
        dynamic_cast<Engine::ImageButton*>(button)->Enabled = true;
        Engine::Point pos = { screenSize.x - OperatorUISize * (opNo + 1) - 109, screenSize.y - OperatorUISize};
        button->Position = pos;
        opNo++;
    }
}

void PlayScene::ConstructOperatorUI() {
    Engine::Point screenSize = Engine::GameEngine::GetInstance().GetScreenSize();
    int i = 0;
    for (auto it = operators.begin(); it != operators.end(); it++) {
        std::string filepath = it->second->getIconPath();
        Engine::Point pos = { screenSize.x - OperatorUISize * (i++ + 1) - 109, screenSize.y - OperatorUISize };
        auto button = new Engine::ImageButton(filepath, filepath, pos.x, pos.y, OperatorUISize, OperatorUISize);
        button->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, it));
        OperatorButtons->AddNewControlObject(button);
        OperatorButtonsFrames->AddNewControlObject(new Engine::ImageButton("Ingame/char.png", "Ingame/charhov.png", pos.x, pos.y, OperatorUISize, OperatorUISize));
    }
}

void PlayScene::ConstructUI() {
    Engine::Point scrSize = Engine::GameEngine::GetInstance().GetScreenSize();
    // Background
    //UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
    // Text
    //UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(new Engine::Image("Ingame/dp ui.png", scrSize.x - 109, scrSize.y - 176));
    UIGroup->AddNewObject(UIDP = new Engine::Label(std::to_string(DP), "pirulen.ttf", 24, scrSize.x - OperatorUISize, scrSize.y - OperatorUISize - 45, 255, 255, 255));
    //UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));
    //UIGroup->AddNewObject(UIScore = new Engine::Label(std::string("Score ") + std::to_string(score), "pirulen.ttf", 24, 1294, 128));
    
    //TODO: render frame and cost
    ConstructOperatorUI();
}

void PlayScene::UIBtnClicked(std::vector<std::pair<float, Operator*>>::iterator it) {
    if (preview)
        FieldGroup->GetBillboards()->RemoveControlObject(preview->GetControlIterator(), preview->GetObjectIterator());
    if (dynamic_cast<Amiya*>(it->second)) {
        preview = new Amiya();
    }
    else if (dynamic_cast<Logos*>(it->second)) {
        preview = new Logos();
    }
    else if (dynamic_cast<Necrass*>(it->second)) {
        preview = new Necrass();
    }
    else if (dynamic_cast<Reed2*>(it->second)) {
        preview = new Reed2();
    }
    else if (dynamic_cast<Wisadel*>(it->second)) {
        preview = new Wisadel();
    }
    else if (dynamic_cast<Cetsyr*>(it->second)) {
        preview = new Cetsyr();
    }
    else if (dynamic_cast<Shu*>(it->second)) {
        preview = new Shu();
    }
    if (!preview)
        return;

    //preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
    //preview->Tint = al_map_rgba(255, 255, 255, 200);
    //preview->Enabled = false;
    //preview->Preview = true;
    curSelectIndex = std::distance(operators.begin(), it);
    PrevSpeedMult = SpeedMult;
    SpeedMult = 0;
    FieldGroup->AddNewControlBillboard(preview);
    //UIGroup->AddNewObject(preview);
    OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

//Should only check if the space is occupied, not occupying the space itself.
//God please.
//Do as the function name say.
std::pair<bool, std::vector<std::vector<int>>> PlayScene::CheckSpaceValid(int x, int y, TileType type) {
    //Check out of bounds
    if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
        return {false, {}};
    if(mapState[y][x] & TILE_SPAWN || mapState[y][x] & TILE_OBJECTIVE) return {false, {}};

    //Check if blocking all enemies' paths
    auto map00 = mapState[y][x];
    mapState[y][x] |= type;
    std::vector<std::vector<int>> map = CalculateBFSDistance();
    mapState[y][x] = map00;
    //Assume path is always found
    /*if (map[SpawnGridPoint.y][SpawnGridPoint.x] == -1)
        return {false, {}};*/
    return {true, map};
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
    // Reverse BFS to find path.
    std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
    std::queue<Engine::Point> que;
    // Push end point.
    // BFS from end point.
    que.push(EndGridPoint);
    map[EndGridPoint.y][EndGridPoint.x] = 0;
    while (!que.empty()) {
        Engine::Point p = que.front();
        que.pop();
        for(auto delta : directions){
            if((p+delta).x < 0 || (p+delta).y < 0) continue;
            if ((p + delta).x >= MapWidth || (p + delta).y >= MapHeight) continue;
            //Bypass highground and blocked tiles
            if(mapState[p.y + delta.y][p.x + delta.x] & TILE_BLOCKED) continue;
            if (mapState[p.y + delta.y][p.x + delta.x] & TILE_HIGH) continue;
            if(map[p.y + delta.y][p.x + delta.x] < 0){
                que.push(p + delta);
                map[p.y + delta.y][p.x + delta.x] = map[p.y][p.x] + 1;
            }
            else{
                map[p.y + delta.y][p.x + delta.x] = std::min(map[p.y][p.x]+ 1, map[p.y + delta.y][p.x + delta.x]);
            }
        }
    }
    return map;
}

void PlayScene::TriggerCheat()
{
    Engine::LOG(Engine::INFO) << "Triggering cheat";
    EarnDP(10000);
    EffectGroup->AddNewObject(new Plane());
}
