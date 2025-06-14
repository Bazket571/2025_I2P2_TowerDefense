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
bool StoryScene::Intro = false;
void StoryScene::Initialize() {
    dialogue = new Engine::Label("","DepartureMono-Regular.otf",20,100,700,255,255,255);
    dialogue->Size.x = 1400;
    speaker = new Engine::Label("","DepartureMono-Regular.otf",24,70,650,255,255,255);
    openingline = new Engine::Label("", "DepartureMono-Regular.otf", 24, 0, 0, 255, 255, 255);
    dialoguebar = new Engine::Image("background/dialoguebar.png",0,0, 1650, 920);
    background = new Engine::Image("background/1-1.png",0,0,1680,920);
    character = new Engine::Image("char/Amiya.png",-120,25,0,0);
    character2 = new Engine::Image("char/Medic.png",550,-200,1300,1300);
    character3 = new Engine::Image("char/Medic.png", 650, 25, 0, 0);
    character4 = new Engine::Image("char/Amiya.png", -100, 25, 0, 0);
    shade = new Engine::Image("background/shade.png", 0, 0, 1680, 920);
    character->Visible = false;
    character2->Visible = false;
    character3->Visible = false;
    character4->Visible = false;
    AddNewObject(background);
    AddNewObject(character);
    AddNewObject(character2);
    AddNewObject(character3);
    AddNewObject(character4);
    AddNewObject(shade);
    shade->Visible = false;
    AddNewObject(dialoguebar);
    AddNewObject(dialogue);
    AddNewObject(speaker);
    cmdFile.open("Resource/plot/"+StageID+".txt");
    //std::string temp; cmdFile >> temp; //Error, somehow
    //bgmInstance = AudioHelper::PlaySample("story1-1.ogg", true, AudioHelper::BGMVolume);
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
        cmdFile.close();
        if (StageID == "1-3B" || StageID == "Intro") {
            IScene::Terminate();
            Engine::GameEngine::GetInstance().ChangeScene("stage-select");
            return;
        }
        dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId = StageID;
        IScene::Terminate();
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
}

void StoryScene::ReadAndExecute()
{
    std::string cmd;
    cmdFile >> cmd;
    if (cmd == "dia") {
        std::string dia; std::getline(cmdFile, dia);
        dialogue->Text = dia;
    }
    else if (cmd == "Doctor") {
        std::string imgFile;
        std::getline(cmdFile, imgFile);
        shade->Visible = true;
        speaker->Visible = false;
        ReadAndExecute();
    }
    else if (cmd == "spamton") {
        std::string imgFile;
        std::getline(cmdFile, imgFile);
        character->Visible = false;
        character2->Visible = false;
        ReadAndExecute();
    }
    else if (cmd == "speLeft") {
        std::string imgFile, speLeft;
        cmdFile >> imgFile;
        std::getline(cmdFile, speLeft);
        speaker->Text = speLeft;
        character->bmp = Engine::Resources::GetInstance().GetBitmap("char/" + imgFile + ".png");
        shade->Visible = false;
        character->Visible = true;
        speaker->Visible = true;
        ReadAndExecute();
    }
    else if (cmd == "speLeft2") {
        std::string imgFile, speLeft2;
        cmdFile >> imgFile;
        std::getline(cmdFile, speLeft2);
        speaker->Text = speLeft2;
        character4->bmp = Engine::Resources::GetInstance().GetBitmap("char/" + imgFile + ".png");
        shade->Visible = false;
        character4->Visible = true;
        speaker->Visible = true;
        ReadAndExecute();
    }
    else if (cmd == "speRight") {
        std::string imgFile, speRight;
        cmdFile >> imgFile; //THIS SHIT IMPORTANT DUMBASS
        std::getline(cmdFile, speRight);
        speaker->Text = speRight;
        character2->bmp = Engine::Resources::GetInstance().GetBitmap("char/"+ imgFile +".png");
        shade->Visible = false;
        character2->Visible = true;
        speaker->Visible = true;
        ReadAndExecute();
    }
    else if (cmd == "speRight2") {
        std::string imgFile, speRight2;
        cmdFile >> imgFile; //THIS SHIT IMPORTANT DUMBASS
        std::getline(cmdFile, speRight2);
        speaker->Text = speRight2;
        character3->bmp = Engine::Resources::GetInstance().GetBitmap("char/" + imgFile + ".png");
        shade->Visible = false;
        character3->Visible = true;
        speaker->Visible = true;
        ReadAndExecute();
    }
    else if (cmd == "end") {
        cmdFile.close();
        if (StageID == "1-3B" || StageID == "Intro") {
            IScene::Terminate();
            Engine::GameEngine::GetInstance().ChangeScene("stage-select");
            return;
        }
        dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId = StageID;
        IScene::Terminate();
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    else if (cmd == "back") {
        std::string imgFile, back;
        cmdFile >> imgFile;
        std::getline(cmdFile, back);
        background->bmp = Engine::Resources::GetInstance().GetBitmap("background/"+imgFile+".png");
        ReadAndExecute();
    }
    else if (cmd == "thanos") {
        std::string imgFile;
        std::getline(cmdFile, imgFile);
        character4->Visible = false;
        character3->Visible = false;
        ReadAndExecute();
    }
}

