#pragma once
#include "GameObject.h"
#include "Game.h"

//Skin of HUD
#include "Timer.h"
#include "Score.h"
#include "Number.h"
#include "Skeleton.h"

#define ANI_SET_OF_HUD_IN_EVERY_SCENE	101010

class HUD : public CGameObject
{
	int World;
	int MarioSpeed;
	float TimeValue;
	int ScoreValue;
	int Life;

	int FirstCardID;
	int SecondCardID;
	int ThirdCardID;

	vector<Number*> Time;
	vector<Number*> Score;

	Skeleton* TimeScoreLife;
	Skeleton* Card;
public:
	HUD();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	vector<int> SeperateTime();
	vector<int> SeperateScore();

};