#include "HUD.h"
#include "Mario.h"
#include "PlayScene.h"

HUD::HUD() 
{
	//Load value from Game.h
	this->LifeValue = CGame::GetInstance()->GetLife();
	this->ScoreValue = CGame::GetInstance()->GetScore();
	this->MarioSpeed = CGame::GetInstance()->GetMarioSpeed();
	this->TimeValue = CGame::GetInstance()->GetTime();
	this->WorldValue = CGame::GetInstance()->GetWorld();	

	//========================Init skin======================

	//Time
	for(int i = 0; i < 3; i++) {
		Number* num = new Number(0);
		Time.push_back(num);
		Time.at(i)->SetPosition(155+(i*8), 450);
	}

	//Score
	for (int i = 0; i < 7; i++) {
		Number* num = new Number(0);
		Score.push_back(num);
		Score.at(i)->SetPosition(75 + (i * 8), 450);
	}

	//Skeleton contains : time, score, life,.... 
	this->TimeScoreLife = new Skeleton(MAIN_SKELETON);

	//Skeleton contains: card
	this->Card = new Skeleton(CARD_SKELETON);

	//life
	this->Life = new Number(0);

	//World 
	this->World = new Number(0);

	//Power
	this->power = new Power(TYPE_NORMAL);

	//Stack
	for (int i = 0; i < 6; i++) {
		Stack* sta = new Stack(TYPE_WHITE);
		stack.push_back(sta);
	}

	//Set position
	TimeScoreLife->SetPosition(10, 435);
	Card->SetPosition(200, 435);
}

void HUD::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = top = right = bottom = 0;
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	TimeValue -= 0.1f;

	DebugOut(L"\n Score %d", ScoreValue);
	int id = CGame::GetInstance()->GetCurrentScene()->GetID();

	vector<int> TimePool = this->SeperateToNumber(TimeValue,3);

	//DebugOut(L"\nTime pool size %d", TimePool.size());

	vector<int> ScorePool = this->SeperateToNumber(CGame::GetInstance()->GetScore(),7);

	//DebugOut(L"\nScore pool size %d", ScorePool.size());

	if (id == PLAY_SCENE) 
	{
		float camx, camy;
		CGame::GetInstance()->GetInstance()->GetPositionCam(camx, camy);
		TimeScoreLife->SetPosition(camx+30, camy+205);
		Card->SetPosition(camx+200, camy+205);
		for (int i = 0; i < Time.size(); i++)
		{
			Time.at(i)->SetPosition(camx + 155 + (i * 8), camy + 220);
			int lastIndex = TimePool.size() - 1 - i;
			Time.at(i)->SetValue(TimePool.at(lastIndex));
		}
		for (int i = 0; i < Score.size(); i++) 
		{
			Score.at(i)->SetPosition(camx + 75 + (i * 9), camy + 220);
			int lastIndex = ScorePool.size() - 1 - i;
			Score.at(i)->SetValue(ScorePool.at(lastIndex));
		}
		for (int i = 0; i < stack.size(); i++) {
			stack.at(i)->SetPosition(camx + 75 + (i * 9), camy + 212);
		}
		power->SetPosition(camx + 129, camy + 212);
	}
}

void HUD::Render() 
{
	TimeScoreLife->Render();
	Card->Render();
	power->Render();
	for (int i = 0; i < Time.size(); i++) {
		Time.at(i)->Render();
	}
	for (int i = 0; i < Score.size(); i++) {
		Score.at(i)->Render();
	}
	for (int i = 0; i < stack.size(); i++) {
		stack.at(i)->Render();
	}
}

vector<int> HUD::SeperateToNumber(int value,int maxsize) {
	vector<int> rs;

	int val = value;

	int delta = maxsize;
		
	while (val > 0)
	{
		int atom = val % 10;
		rs.push_back(atom);
		val = val / 10;
	}
	
	while (rs.size() < delta)
	{
		rs.push_back(0);
	}
	return rs;
}