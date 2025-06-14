#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "StoryScene.h"
#include "StageSelectScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

std::unordered_map<std::string, bool> StageSelectScene::isComplete;

void StageSelectScene::Initialize() {
    if (isComplete.empty()) {
        isComplete["1-1"] = false;
        isComplete["1-2"] = false;
        isComplete["1-3"] = false;
    }
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton *btn;
    AddNewObject(new Engine::Image("Stage/stage.png", 0, 0, w, h));

    //btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 50, 400, 100);
    //btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
    //AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("Stage 1", "pirulen.ttf", 48, halfW, halfH / 2, 0, 0, 0, 255, 0.5, 0.5));
    //btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 + 100, 400, 100);
    //btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 2));
    //AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("Stage 2", "pirulen.ttf", 48, halfW, halfH / 2 + 150, 0, 0, 0, 255, 0.5, 0.5));
    btn = new Engine::ImageButton("svgs/regular/circle-left.svg", "svgs/solid/circle-arrow-left.svg", 10, 10, 50, 50);
    btn->SetOnClickCallback(std::bind(&StageSelectScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    //// Not safe if release resource while playing, however we only free while change scene, so it's fine.
    //bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);

    std::vector<Engine::Point> imgPos = { {357, 298} , {668, 364}, {989, 423} };
    int i = 0;
    for (std::string id : {"1-1", "1-2", "1-3"}) {
        std::string img = "Stage/" + id + " ";
        if (isComplete[id]) img += "fin";
        btn = new Engine::ImageButton(img + ".png", img + "hov.png", imgPos[i].x, imgPos[i].y);
        btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, id));
        AddNewControlObject(btn);
        i++;
    }
}
void StageSelectScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void StageSelectScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void StageSelectScene::PlayOnClick(std::string stage) {
    StoryScene *scene = dynamic_cast<StoryScene*>(Engine::GameEngine::GetInstance().GetScene("story"));
    scene->StageID = stage;
    Engine::GameEngine::GetInstance().ChangeScene("story");
}
void StageSelectScene::ScoreboardOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("scoreboard-scene");
}
void StageSelectScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}
void StageSelectScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
