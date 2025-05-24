#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"
#include <Engine/spine/spine.hpp>

void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    //AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
    int score = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->score;
    AddNewObject(new Engine::Label("Score: " + std::to_string(score), "pirulen.ttf", 48, halfW, halfH / 4 + 50, 255, 255, 255, 255, 0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");

    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 200, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::ScoreOnClick, this, score));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Scores", "pirulen.ttf", 48, halfW, halfH * 7 / 4 - 150, 0, 0, 0, 255, 0.5, 0.5));

    nameLabel = new Engine::Label("Your name: ", "pirulen.ttf", 30, halfW - 200, halfH * 7 / 4 - 300, 255, 255, 255);
    AddNewObject(nameLabel);

    Engine::SpineSprite* supremeLeader;
    supremeLeader = new Engine::SpineSprite("amiya/build_char_002_amiya.skel", "amiya/build_char_002_amiya.atlas", halfW, halfH, 0.5, 0.5, 0.5);
    supremeLeader->state->setAnimation(1, "Interact", true);
    AddNewObject(supremeLeader);
}
void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
    IScene::Update(deltaTime);
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}
void WinScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

char allegroKeyCodeToChar(int keyCode) {
    if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
        return keyCode - ALLEGRO_KEY_A + 'a';
    }
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        return keyCode - ALLEGRO_KEY_0 + '0';
    }
    return 0;
}

void WinScene::OnKeyDown(int keyCode) {
    char c = allegroKeyCodeToChar(keyCode);
    if (keyCode == ALLEGRO_KEY_BACKSPACE) name.pop_back();
    else if (c == 0) return;
    else name += c;
    nameLabel->Text = "Your name: " + name;
}

void WinScene::ScoreOnClick(int score)
{
    Engine::GameEngine::GetInstance().ChangeScene("score");
}
