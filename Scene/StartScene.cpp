//
// Created by Hsuan on 2024/4/10.
//

#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Engine/spine/spine.hpp"
#include "PlayScene.hpp"
#include "Scene/StartScene.h"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "StoryScene.h"

// TODO HACKATHON-2 (1/3): You can imitate the 2 files: 'StartScene.hpp', 'StartScene.cpp' to implement your SettingsScene.
void StartScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;

    /*AddNewObject(new Engine::Label("Tower Defense", "pirulen.ttf", 120, halfW, halfH / 3 + 50, 10, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 200, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Play", "pirulen.ttf", 48, halfW, halfH / 2 + 250, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 3 / 2 + 100, 400, 100);
    btn->SetOnClickCallback(std::bind(&StartScene::ScoreOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Score", "pirulen.ttf", 48, halfW, halfH * 3 / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));*/
    AddNewObject(new Engine::Image("MainMenu/main menu.png", 0, 0, w, h));

    btn = new Engine::ImageButton("MainMenu/start.png", "MainMenu/starthov.png", halfW - 108 / 2 - 25, halfH * 1.5);
    btn->SetOnClickCallback(std::bind(&StartScene::PlayOnClick, this, 0));
    AddNewControlObject(btn);

    btn = new Engine::ImageButton("MainMenu/cred.png", "MainMenu/credhov.png", halfW - 150 / 2 - 25, halfH * 1.5 + 50);
    btn->SetOnClickCallback(std::bind(&StartScene::SettingsOnClick, this, 0));
    AddNewControlObject(btn);
}
void StartScene::Terminate() {
    IScene::Terminate();
}
void StartScene::PlayOnClick(int stage) {
    if (!StoryScene::Intro) {
        StoryScene::Intro = true;
        StoryScene* scene = dynamic_cast<StoryScene*>(Engine::GameEngine::GetInstance().GetScene("story"));
        scene->StageID = "Intro";
        Engine::GameEngine::GetInstance().ChangeScene("story");
        return;
    }
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void StartScene::SettingsOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("settings");
}

void StartScene::ScoreOnClick()
{
    Engine::GameEngine::GetInstance().ChangeScene("score");
}
