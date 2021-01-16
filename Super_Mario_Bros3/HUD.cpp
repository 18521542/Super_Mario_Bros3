#include "HUD.h"
#include "Mario.h"
#include "PlayScene.h"

#define EACH_NUMBER_DISTANCE				8

#define NUMBERS_USED_AS_SKIN_OF_TIME		3
#define NUMBERS_USED_AS_SKIN_OF_SCORE		7

#define POSY_OF_SKELETON_MAIN				210
#define	POSX_OF_SKELETON_MAIN				30	

#define POSY_OF_SKELETON_CARD				210
#define	POSX_OF_SKELETON_CARD				200

#define POSY_OF_FIRST_NUMBER_TIME			225
#define	POSX_OF_FIRST_NUMBER_TIME			155

#define POSY_OF_FIRST_NUMBER_SCORE			225
#define	POSX_OF_FIRST_NUMBER_SCORE			75

#define POSY_OF_FIRST_STACK					217
#define	POSX_OF_FIRST_STACK					75

#define POSY_OF_POWER						217
#define	POSX_OF_POWER						129

#define POSY_OF_BACKGROUND					205
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
		Time.at(i)->SetPosition(155+(i* EACH_NUMBER_DISTANCE), 450);
	}

	//Score
	for (int i = 0; i < 7; i++) {
		Number* num = new Number(0);
		Score.push_back(num);
		Score.at(i)->SetPosition(75 + (i * EACH_NUMBER_DISTANCE), 450);
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
		Stack* sta = new Stack(TYPE_BLACK, i);
		stack.push_back(sta);
	}

	//Set position
	TimeScoreLife->SetPosition(10, 435);
	Card->SetPosition(200, 435);
	BlackBackGround = new CBackground();

	FCard = new HUD_Card(PLANT);
	FCard->SetIsAppear(true);
}

void HUD::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = top = right = bottom = 0;
}


void HUD::TimeUpdate(float camX, float camY) 
{
	vector<int> TimePool = this->SeperateToNumber(TimeValue, NUMBERS_USED_AS_SKIN_OF_TIME);

	for (int i = 0; i < Time.size(); i++)
	{
		Time.at(i)->SetPosition(camX + POSX_OF_FIRST_NUMBER_TIME + (i * EACH_NUMBER_DISTANCE), camY + POSY_OF_FIRST_NUMBER_TIME);
		int lastIndex = TimePool.size() - 1 - i;
		Time.at(i)->SetValue(TimePool.at(lastIndex));
	}
}
void HUD::ScoreUpdate(float camX, float camY) 
{
	vector<int> ScorePool = this->SeperateToNumber(CGame::GetInstance()->GetScore(), NUMBERS_USED_AS_SKIN_OF_SCORE);
	for (int i = 0; i < Score.size(); i++)
	{
		Score.at(i)->SetPosition(camX + POSX_OF_FIRST_NUMBER_SCORE + (i * (EACH_NUMBER_DISTANCE+1)), camY + POSY_OF_FIRST_NUMBER_SCORE);
		int lastIndex = ScorePool.size() - 1 - i;
		Score.at(i)->SetValue(ScorePool.at(lastIndex));
	}
}
void HUD::StackUpdate(float camX, float camY, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (StackLevel < 0)
		StackLevel = 0;
	if (StackLevel > 6)
		StackLevel = 6;

	if (StackIsUp) {
		StackUp();
	}
	else {
		StackDown();
	}
	for (int i = 0; i < stack.size(); i++) 
	{
		stack.at(i)->SetPosition(camX + POSX_OF_FIRST_NUMBER_SCORE + (i * (EACH_NUMBER_DISTANCE + 1)), camY + POSY_OF_FIRST_STACK);
		if (stack[i]->GetLevel() < this->StackLevel) {
			stack[i]->SetType(TYPE_WHITE);
		}
		else {
			stack[i]->SetType(TYPE_BLACK);
		}
	}
}

void HUD::StackUp() {
	
	if (!StackIsUp) {
		//time = 0;
		return;
	}
	//hard code
	if (GetTickCount64() - StartUptime>170) {
		StackLevel++;
		StartUptime = GetTickCount64();
	}
}

void HUD::StackDown() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsFlying())
		return;
	if (StackIsUp ) {
		return;
	}
	if (GetTickCount64() - StartDownTime > 200) {
		StackLevel--;
		StartDownTime = GetTickCount64();
	}
}



void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	TimeValue -= 0.01f;

	int id = CGame::GetInstance()->GetCurrentScene()->GetID();

	if (id == WORLDMAP_SCENE)
	{
		TimeValue = 0;
		power->SetType(TYPE_NORMAL);
	}

	float camx, camy;
	CGame::GetInstance()->GetInstance()->GetPositionCam(camx, camy);

	//Update position
	BlackBackGround->SetPosition(camx, camy + POSY_OF_BACKGROUND);
	TimeScoreLife->SetPosition(camx + POSX_OF_SKELETON_MAIN, camy + POSY_OF_SKELETON_MAIN);
	Card->SetPosition(camx + POSX_OF_SKELETON_CARD, camy + POSY_OF_SKELETON_MAIN);
	power->SetPosition(camx + POSX_OF_POWER, camy + POSY_OF_POWER);
	FCard->SetPosition(camx + POSX_OF_SKELETON_CARD+1, camy + POSY_OF_SKELETON_MAIN+1);


	//update state
	TimeUpdate(camx, camy);
	ScoreUpdate(camx, camy);
	StackUpdate(camx, camy, dt, coObjects);
	FCard->Update(dt, coObjects);
	power->Update(dt, coObjects);
	FCard->SetID(CGame::GetInstance()->GetFCardID());
}

void HUD::Render() 
{
	BlackBackGround->Render();
	TimeScoreLife->Render();
	Card->Render();
	power->Render();
	FCard->Render();
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