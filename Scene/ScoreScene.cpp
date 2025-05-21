#include "ScoreScene.hpp"
#include <fstream>
#include <algorithm>
#include <UI/Component/Label.hpp>
#include <Engine/GameEngine.hpp>
#include <Scene/PlayScene.hpp>

const int ScoreScene::entriesPerPage = 7;
const int ScoreScene::scoreboardHeightOffset = 100;

std::vector<ScoreEntry> ScoreScene::readScoreFile(std::string filename)
{
    std::ifstream file(filename);
    std::vector<ScoreEntry> ret;
    while(!file.eof()){
        ScoreEntry entry;
        file>>entry.name>>entry.score>>entry.timestamp;
        ret.push_back(entry);
    }
    std::sort(ret.begin(), ret.end(), [](const ScoreEntry& a, const ScoreEntry& b){
        return a.score > b.score;
    });
    file.close();
    return ret;
}
void ScoreScene::writeScoreFile(const std::vector<ScoreEntry> &entries, const std::string filename)
{
    std::ofstream file(filename);
    for(ScoreEntry entry : entries){
        file<<entry.name<<' '<<entry.score<<' '<<entry.timestamp<<std::endl;
    }
    file.close();
}
void ScoreScene::drawScores(int page)
{
    for(auto label : labels){
        for(auto item : label)
            item->Text.clear();
    }
    for(int i = entriesPerPage * page; i < entriesPerPage * (page + 1) && i < entries.size(); i++){
        labels[i - entriesPerPage * page][0]->Text = entries[i].name;
        labels[i - entriesPerPage * page][1]->Text = std::to_string(entries[i].score);
        labels[i - entriesPerPage * page][2]->Text = std::ctime(&entries[i].timestamp);
    }
}
void ScoreScene::Initialize()
{
    score = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->score;
    currentPage = 0;
    entries = readScoreFile("Resource/scoreboard.txt");
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("svgs/regular/circle-left.svg", "svgs/solid/circle-arrow-left.svg", 10, 10, 50, 50);
    btn->SetOnClickCallback(std::bind(&ScoreScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("SCOREBOARD", "pirulen.ttf", 48, w / 2, 50, 255, 255, 255, 255, 0.5, 0.5));

    labels.clear();
    labels.resize(entriesPerPage);
    for(int i = 0; i < entriesPerPage; i++){
        labels[i][0] = new Engine::Label("", "pirulen.ttf", 24, w / 2 - 500, scoreboardHeightOffset + i * 35, 255, 255, 255, 255, 0, 0.5);
        labels[i][1] = new Engine::Label("", "pirulen.ttf", 24, w / 2 - 200, scoreboardHeightOffset + i * 35, 255, 255, 255, 255, 0, 0.5);
        labels[i][2] = new Engine::Label("", "pirulen.ttf", 24, w / 2, scoreboardHeightOffset + i * 35, 255, 255, 255, 255, 0, 0.5);
        for(Engine::Label* l : labels[i]) AddNewObject(l);
    }

    prevPage = new Engine::ImageButton("svgs/solid/angles-left.svg", "svgs/solid/angles-left.svg", w/2 - 200, 700, 50, 50);
    prevPage->SetOnClickCallback([this](){currentPage--; drawScores(currentPage);});
    nextPage = new Engine::ImageButton("svgs/solid/angles-right.svg", "svgs/solid/angles-right.svg", w/2 + 200, 700, 50, 50);
    nextPage->SetOnClickCallback([this](){currentPage++; drawScores(currentPage);});

    AddNewControlObject(prevPage);
    AddNewControlObject(nextPage);
    drawScores(0);

}
void ScoreScene::Terminate(){
    writeScoreFile(entries, "Resource/scoreboard.txt");
    IScene::Terminate();    
}
void ScoreScene::Update(float deltaTime){
    IScene::Update(deltaTime);
    prevPage->Visible = currentPage > 0;
    nextPage->Visible = currentPage < entries.size() / entriesPerPage;
}
void ScoreScene::BackOnClick(int stage){
    Engine::GameEngine::GetInstance().ChangeScene("start");
}