#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "StoryScene.h"

void StoryScene::Initialize() {
    dialogue = new Engine::Label("","pirulen.ttf",20,100,700,255,255,255);
    speaker = new Engine::Label("","pirulen.ttf",24,70,650,255,255,255);
    dialoguebar = new Engine::Image("background/dialoguebar.png",0,0, 1680, 920);
    background = new Engine::Image("background/background1-1.png",0,0,1680,920);
    character = new Engine::Image("char/Amiya.png",-120,25,0,0);
    character2 = new Engine::Image("char/Medic.png",700,25,0,0);
    character->Visible = false;
    character2->Visible = false;
    AddNewObject(background);
    AddNewObject(character);
    AddNewObject(character2);
    AddNewObject(dialoguebar);
    AddNewObject(dialogue);
    AddNewObject(speaker);
    cmdFile.open("Resource/plot/1-1.txt");
    bgmInstance = AudioHelper::PlaySample("story1-1.ogg", true, AudioHelper::BGMVolume);
    ReadAndExecute();
}

void StoryScene::OnMouseDown(int button, int mx, int my)
{
    ReadAndExecute();
}

void StoryScene::OnKeyDown(int keyCode)
{
    if (keyCode == ALLEGRO_KEY_SPACE) {
        ReadAndExecute();
    }
    if (keyCode == ALLEGRO_KEY_ESCAPE) {
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
}

void StoryScene::ReadAndExecute()
{
    std::string cmd;
    cmdFile >> cmd;
    if (cmd == "play") {
        std::string target;
        cmdFile >> target;
        PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
        scene->MapId = target;
    }
    else if (cmd == "dia") {
        std::string dia; std::getline(cmdFile, dia);
        dialogue->Text = dia;
    }
    else if (cmd == "speLeft") {
        std::string imgFile, speLeft;
        cmdFile >> imgFile;
        std::getline(cmdFile, speLeft);
        speaker->Text = speLeft;
        character->bmp = Engine::Resources::GetInstance().GetBitmap(imgFile);
        character->Visible = true;
        speaker->Visible = true;
        if (speLeft == "none") {
            character->Visible = false;
            speaker->Visible = false;
        };
        ReadAndExecute();
    }
    else if (cmd == "speRight") {
        std::string imgFile, speRight;
        cmdFile >> imgFile; //THIS SHIT IMPORTANT DUMBASS
        std::getline(cmdFile, speRight);
        speaker->Text = speRight;
        character2->bmp = Engine::Resources::GetInstance().GetBitmap(imgFile);
        character2->Visible = true;
        speaker->Visible = true;
        if (speRight == "none") {
            character2->Visible = false;
            speaker->Visible = false;
        };
        ReadAndExecute();
    }
    else if (cmd == "end") {
        AudioHelper::StopSample(bgmInstance);
        bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
        IScene::Terminate();
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    else if (cmd == "back") {
        std::string back; std::getline(cmdFile, back);
        background->bmp = Engine::Resources::GetInstance().GetBitmap("background/background1-1");
    }
}

