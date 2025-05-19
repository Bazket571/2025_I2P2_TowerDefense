#pragma once
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class ScoreScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;

public:
    explicit ScoreScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
};
