#pragma once
#include "GameObject.h"
#include "Game.h"

//Skin of HUD
#include "Number.h"
#include "Skeleton.h"
#include "Background.h"
#include "Power.h"
#include "Stack.h"
#include "HUD_Card.h"

#define ANI_SET_OF_HUD_IN_EVERY_SCENE	101010

class HUD : public CGameObject
{
	int WorldValue;
	int MarioSpeed;
	float TimeValue ;
	int ScoreValue;
	int LifeValue;

	int FirstCardID;
	int SecondCardID;
	int ThirdCardID;

	bool StackIsUp = false;
	ULONGLONG StartUptime;
	ULONGLONG StartDownTime;

	CBackground* BlackBackGround;

	vector<Number*> Time; // times contains 3 numbers
	vector<Number*> Score; // scores contains 7 numbers

	Number* Life;	// life only need 1 number
	Number* World;  // so does world

	Skeleton* TimeScoreLife;
	Skeleton* Card;

	HUD_Card* FCard;

	Power* power; // only 1P

	vector<Stack*> stack; // stack contain 6 level  
	DWORD time;
	int StackLevel = 0;
public:
	HUD();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void TimeUpdate(float camX, float camY);
	void StackUpdate(float camX, float camY, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ScoreUpdate(float camX, float camY);

	//stack manage - need a class to manage stack atoms
	void StackUp();
	void StackDown();
	void StackStartUp() { StartUptime = GetTickCount64(); StackIsUp = true; }
	void StackStartDown() { StartDownTime = GetTickCount64(); StackIsUp = false; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	
	vector<int> SeperateToNumber(int value,int maxsize);
};