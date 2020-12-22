#pragma once

#define LEFT					-1
#define RIGHT					1
#define UP						-1
#define DOWN					1

#define PIRANHA_PLANT			1
#define VENUS_RED				2
#define VENUS_GREEN				3

#define VENUS_WIDTH_BB			16
#define VENUS_HEIGHT_BB			32

#define PIRANHA_WIDTH_BB		16
#define PIRANHA_HEIGHT_BB		24

#define VENUS_RED_ANI_UP_LEFT		0
#define VENUS_RED_ANI_DOWN_LEFT		1
#define VENUS_RED_ANI_UP_RIGHT		2
#define VENUS_RED_ANI_DOWN_RIGHT	3

#define VENUS_GREEN_ANI_UP_LEFT		4
#define VENUS_GREEN_ANI_DOWN_LEFT	5
#define VENUS_GREEN_ANI_UP_RIGHT	6
#define VENUS_GREEN_ANI_DOWN_RIGHT	7

#define PIRANHA_ANI					8


#include "GameObject.h"
#include "Utils.h"
#include "Object.h"

class CVenus : public CGameObject
{
	bool isAppear = false;
	int ny;
	int type;

	CFireBall* fireball;
	bool isInitFB = false;
	bool HasFired = false;

	int ReadyToUp = -1;
	bool isAllowToMove = true;
	bool isMoving = false;
	ULONGLONG TimeMovingY;
	ULONGLONG TimeBlockMoving;

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	CVenus(int type);
	void SetDirection(int dir) { this->nx = dir; }
	virtual void SetState(int state);
	void StartBlock() { TimeBlockMoving = GetTickCount64(); this->isAllowToMove = false; isMoving = false; ReadyToUp = -ReadyToUp; }
	void StartMoving() { TimeMovingY = GetTickCount64(); this->isAllowToMove = true; isMoving = true; }
};