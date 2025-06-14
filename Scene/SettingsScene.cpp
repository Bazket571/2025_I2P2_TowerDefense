#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "Engine/LOG.hpp"
#include "PlayScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "SettingsScene.hpp"

void SettingsScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("svgs/regular/circle-left.svg", "svgs/solid/circle-arrow-left.svg", 10, 10, 50, 50);
    btn->SetOnClickCallback(std::bind(&SettingsScene::BackOnClick, this, 1));

    AddNewObject(new Engine::Image("credit page.png", 0, 0, w, h));
    AddNewControlObject(btn);

}
void SettingsScene::Terminate() {
    //AudioHelper::StopSample(bgmInstance);
    //bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void SettingsScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void SettingsScene::ToggleBoundingBoxClick()
{
    Engine::GameEngine::GetInstance().GetSettings().ToggleBoundingBox();
}
void SettingsScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
    //Engine::LOG(Engine::INFO) << "Change BGM volume to " << value;
}
void SettingsScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}
