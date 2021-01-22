#pragma once
#include "GameObject.h"
#include "Boomerang.h"
#include "Utils.h"

#define STATE_FIRST_FIRING	1
#define STATE_SHORT_WAITING	2
#define STATE_SECOND_FIRING	3
#define STATE_LONG_WAITING	4
class BoomerangBrother : public CGameObject 
{
	Boomerang* FirstWeapon;
	Boomerang* SecondWeapon;

	ULONGLONG MovingLeft;
	ULONGLONG MovingRight;

	ULONGLONG LongWaitingTimeStart;
	ULONGLONG FirstFiringTimeStart;
	ULONGLONG ShortWaitingTimeStart;
	ULONGLONG SecondFiringStart;


	bool isMovingLeft;

	bool isFirstFiring = false;
	bool isSecondFiring = false;
	bool isShortWaiting = false;
	bool isLongWaiting = false;

	bool isMovingHand = false;

	bool isInitedWeapon = false;

	
public:
	bool isDead = false;
	BoomerangBrother();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int stat) { this->state = stat; }

	//State Machine
	void StartFirstFiring() { FirstFiringTimeStart = GetTickCount64(); state = STATE_FIRST_FIRING;  }
	void StartSecondFiring() { SecondFiringStart = GetTickCount64(); state = STATE_SECOND_FIRING; }
	void StartShortWaiting() { ShortWaitingTimeStart = GetTickCount64(); state = STATE_SHORT_WAITING; }
	void StartLongWaiting() { LongWaitingTimeStart = GetTickCount64(); state = STATE_LONG_WAITING; }

	void StateMachine(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void StartMoving();
};