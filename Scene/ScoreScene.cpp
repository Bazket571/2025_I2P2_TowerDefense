#include "ScoreScene.hpp"
#include <UI/Component/ImageButton.hpp>
#include <Engine/GameEngine.hpp>

void ScoreScene::Initialize(){
    IScene::Initialize();
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("svgs/regular/circle-left.svg", "svgs/solid/circle-arrow-left.svg", 10, 10, 50, 50);
    btn->SetOnClickCallback(std::bind(&ScoreScene::BackOnClick, this, 1));
    AddNewControlObject(btn);

}
void ScoreScene::Terminate(){
    IScene::Terminate();    
}
void ScoreScene::Update(float deltaTime){
    IScene::Update(deltaTime);
}
void ScoreScene::BackOnClick(int stage){
    Engine::GameEngine::GetInstance().ChangeScene("start");
}