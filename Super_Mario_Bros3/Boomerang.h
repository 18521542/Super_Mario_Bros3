#pragma once
#include "GameObject.h"
#define BOM_BRO_ANI_SET			129101

#define STATE_MOVING		1
#define STATE_NOT_MOVING	2

#define ANI_POS_MOVING		2
#define ANI_POS_NOTMOVING	3

#define UP_RIGHT			1
#define DOWN_RIGHT			2
#define DOWN_LEFT			3
#define	UP_LEFT				4

class Boomerang : public CGameObject {
	
	bool isMoving = false;
	bool isWaiting = false;
	bool isResetSpeed = false;
	bool isCollide = false;

	
	ULONGLONG TimeMovingStart;
	ULONGLONG TimeWaitingToMove;

	bool isMovingUpRight = false;
	bool isMovingDownRight = false;
	bool isMovingUpLeft = false;
	bool isMovingDownLeft = false;

	int trajectory;
	ULONGLONG MovingUpRight;
	ULONGLONG MovingDownRight;
	ULONGLONG MovingUpLeft;
	ULONGLONG MovingDownLeft;

	ULONGLONG TimeCollide = 0;
	//ULONGLONG MovingUp;
public :
	bool isStopUpdate = false;
	bool isActive = false;
	bool isAllowCollideOneTime = true;
	Boomerang();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int stat) { this->state = stat; }
	void StartMoving() { TimeMovingStart = GetTickCount64(); isMoving = true; state = STATE_MOVING; TimeWaitingToMove = GetTickCount64();  StartMovingUpRight(); }
	void StartWaitingToMove() { TimeWaitingToMove = GetTickCount64(); isWaiting = true; }

	void StartMovingUpRight() { MovingUpRight = GetTickCount64(); isMovingUpRight = true; trajectory = UP_RIGHT; }
	void StartMovingDownRight() { MovingDownRight = GetTickCount64(); isMovingDownRight = true; trajectory = DOWN_RIGHT; }
	void StartMovingUpLeft() { MovingUpLeft = GetTickCount64(); isMovingUpLeft = true; trajectory = UP_LEFT; }
	void StartMovingDownLeft() { MovingDownLeft = GetTickCount64(); isMovingDownLeft = true; trajectory = DOWN_LEFT;  }

	bool IsMoving() { return this->isMoving; }
};