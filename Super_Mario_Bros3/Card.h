#pragma once
#include "GameObject.h"
#include "HUD_Card.h"
#include "Utils.h"

#define	ANI_POS_STAR_IS_NOT_EATEN		0
#define	ANI_POS_PLANT_IS_NOT_EATEN		1
#define	ANI_POS_MUSHROOM_IS_NOT_EATEN	2

#define	ANI_POS_STAR_IS_EATEN			3
#define	ANI_POS_PLANT_IS_EATEN			4
#define	ANI_POS_MUSHROOM_IS_EATEN		5
#define ANI_POS_LETTER_END				6

#define STAR							1
#define PLANT							2
#define MUSHROOM_CARD					3

class Card :public CGameObject {
	int id;
	bool isEatenByMario = false;
	bool isAppear = true;
	bool isMoving = false;
	bool isRandomFirstTime = true;
	bool isAllowToRandom = true;
	bool isInitStart = false;
	ULONGLONG	StartMovingTime;
	ULONGLONG	StartRandomTime;
	HUD_Card* hud_card;
public:
	Card();
	int RandomID();
	bool IsMoving() { return isMoving; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void StartMoving() { StartMovingTime = GetTickCount64(); isMoving = true; DebugOut(L"\n cO NHAY VO DAY %d", isMoving);}
	void StopRandom() { isAllowToRandom = false; }
	int GetID() { return this->id; }
};