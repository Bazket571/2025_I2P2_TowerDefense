#pragma once
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <memory>
#include <fstream>

class StoryScene : public Engine::IScene {
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    Engine::Label* dialogue;
    Engine::Label* speaker;
    Engine::Image* background;
    Engine::Image* character;
    Engine::Image* character2;
    Engine::Image* character3;
    Engine::Image* character4;
    Engine::Image* dialoguebar;
    Engine::Label* openingline;
    Engine::Image* shade;
    //Engine::ImageButton* skip;
    std::ifstream cmdFile;
    void Initialize() override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void ReadAndExecute();
public:
    static bool Intro;
    std::string StageID;
};