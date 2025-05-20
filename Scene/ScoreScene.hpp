#pragma once
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <ctime>
#include <vector>
#include <string>
#include <array>

#include <UI/Component/Label.hpp>

struct ScoreEntry{
    std::string name;
    int score;
    std::time_t timestamp;
};

class ScoreScene final : public Engine::IScene {
private:
    static const int entriesPerPage;
    static const int scoreboardHeightOffset;
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    std::vector<ScoreEntry> entries;
    std::vector<std::array<Engine::Label*, 3>> labels;
    int score;
    std::vector<ScoreEntry> readScoreFile(std::string filename);
    void writeScoreFile(const std::vector<ScoreEntry>& entries);
    void drawScores(int page);

public:
    explicit ScoreScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
};
