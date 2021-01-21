#include "BoomerangBrother.h"
#define BOM_BRO_STATE_WALKING	1
#define BOM_BRO_STATE_FIRING	2

#include "Utils.h"

#define TO_HAND_POS_X	5.0f
#define TO_HAND_POS_Y	5.0f


BoomerangBrother::BoomerangBrother() {
	FirstWeapon = new Boomerang();
	SecondWeapon = new Boomerang();
	//StartWaitingToFiring();
	StartLongWaiting();
	
}
void BoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom) {

}
void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	//SecondWeapon->Update(dt,coObjects);

	CGameObject::Update(dt, coObjects);
	x += dx;

	StartMoving();
	
	FirstWeapon->Update(dt, coObjects);
	SecondWeapon->Update(dt, coObjects);

	if (FirstWeapon->GetState() == STATE_NOT_MOVING) 
	{
		FirstWeapon->SetPosition(x- TO_HAND_POS_X, y- TO_HAND_POS_Y);
		FirstWeapon->isAllowCollideOneTime = true;
	}

	if (SecondWeapon->GetState() == STATE_NOT_MOVING) {
		SecondWeapon->SetPosition(x - TO_HAND_POS_X, y - TO_HAND_POS_Y);
		SecondWeapon->isAllowCollideOneTime = true;
	}
	StateMachine(dt, coObjects);
	if (!isInitedWeapon) {
		isInitedWeapon = true;
		FirstWeapon->SetPosition(x, y);
		SecondWeapon->SetPosition(x, y);
	}
}

#define LONG_WAIT_TIME	3000
#define TIME_FOR_FIRST_FIRING	500
#define SHORT_WAIT_TIME	1000
#define TIME_FOR_SECOND_FIRING	500
void BoomerangBrother::StateMachine(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state == STATE_LONG_WAITING)
	{

		if (GetTickCount64() - LongWaitingTimeStart > LONG_WAIT_TIME) {
			StartFirstFiring();
			
		}
	}
	else if (state == STATE_FIRST_FIRING) {

		FirstWeapon->StartWaitingToMove();

		if (GetTickCount64() - FirstFiringTimeStart > TIME_FOR_FIRST_FIRING) {
			StartShortWaiting();
		}
	}
	else if (state == STATE_SHORT_WAITING) {
	
		if (GetTickCount64() - ShortWaitingTimeStart > SHORT_WAIT_TIME)
		{
			StartSecondFiring();
		}
	}
	else if (state == STATE_SECOND_FIRING) {
		
	    SecondWeapon->StartWaitingToMove();

		if (GetTickCount64() - SecondFiringStart > TIME_FOR_SECOND_FIRING) {
			StartLongWaiting();
		}
	}
}

#define TIME_MOVING_UNTILL_CHANGE_DIRECTION	1500
#define SPEED_X	0.02f
void BoomerangBrother::StartMoving() {
	if (GetTickCount64() - MovingLeft > TIME_MOVING_UNTILL_CHANGE_DIRECTION && isMovingLeft)
	{
		isMovingLeft = false;
		MovingRight = GetTickCount64();
	}

	if (GetTickCount64() - MovingRight > TIME_MOVING_UNTILL_CHANGE_DIRECTION && !isMovingLeft) {
		isMovingLeft = true;
		MovingLeft = GetTickCount64();
	}

	if (isMovingLeft) {
		vx = -SPEED_X;
	}
	else
		vx = SPEED_X;
}
void BoomerangBrother::Render() 
{
	int ani = -1;
		
	if (state == STATE_SHORT_WAITING || state == STATE_LONG_WAITING) {
		ani = 0;
	}
	else if (state == STATE_FIRST_FIRING || state == STATE_SECOND_FIRING) {
		ani = 1;
	}

	animation_set->at(ani)->Render(x, y);
	
	FirstWeapon->Render();
	SecondWeapon->Render();
}

