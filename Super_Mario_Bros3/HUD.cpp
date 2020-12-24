#include "HUD.h"
#include "Mario.h"
#include "PlayScene.h"

HUD::HUD() 
{
	//Load value from Game.h
	this->Life = CGame::GetInstance()->GetLife();
	this->ScoreValue = CGame::GetInstance()->GetMarioSpeed();
	this->MarioSpeed = CGame::GetInstance()->GetScore();
	this->TimeValue = CGame::GetInstance()->GetTime();
	this->World = CGame::GetInstance()->GetWorld();	

	//Init skin
	for(int i = 0; i < 3; i++) {
		Number* num = new Number(0);
		Time.push_back(num);
	}
	for (int i = 0; i < 8; i++) {
		Number* num = new Number(0);
		Score.push_back(num);
	}
	this->TimeScoreLife = new Skeleton(MAIN_SKELETON);
	this->Card = new Skeleton(CARD_SKELETON);
	TimeScoreLife->SetPosition(10, 435);
	Card->SetPosition(200, 435);
}

void HUD::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = top = right = bottom = 0;
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	int id = CGame::GetInstance()->GetCurrentScene()->GetID();
	if (id == PLAY_SCENE) 
	{
		float camx, camy;
		CGame::GetInstance()->GetInstance()->GetPositionCam(camx, camy);
		TimeScoreLife->SetPosition(camx+30, camy+205);
		Card->SetPosition(camx+200, camy+205);
	}
}


void HUD::Render() 
{
	TimeScoreLife->Render();
	Card->Render();
}