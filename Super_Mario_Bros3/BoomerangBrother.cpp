#include "BoomerangBrother.h"
#define BOM_BRO_STATE_WALKING	1
#define BOM_BRO_STATE_FIRING	2

#include "Utils.h"


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
		FirstWeapon->SetPosition(x-5.0f, y-5.0f);
	}

	if (SecondWeapon->GetState() == STATE_NOT_MOVING) {
		SecondWeapon->SetPosition(x - 5.0f, y - 5.0f);
	}
	StateMachine(dt, coObjects);
	if (!isInitedWeapon) {
		isInitedWeapon = true;
		FirstWeapon->SetPosition(x, y);
		SecondWeapon->SetPosition(x, y);
	}
}

void BoomerangBrother::StateMachine(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (state == STATE_LONG_WAITING)
	{

		if (GetTickCount64() - LongWaitingTimeStart > 3000) {
			StartFirstFiring();
			
		}
	}
	else if (state == STATE_FIRST_FIRING) {

		FirstWeapon->StartWaitingToMove();

		if (GetTickCount64() - FirstFiringTimeStart > 500) {
			StartShortWaiting();
		}
	}
	else if (state == STATE_SHORT_WAITING) {
	
		if (GetTickCount64() - ShortWaitingTimeStart > 1000) 
		{
			StartSecondFiring();
		}
	}
	else if (state == STATE_SECOND_FIRING) {
		
	    SecondWeapon->StartWaitingToMove();

		if (GetTickCount64() - SecondFiringStart > 500) {
			StartLongWaiting();
		}
	}
}
void BoomerangBrother::StartMoving() {
	if (GetTickCount64() - MovingLeft > 1500 && isMovingLeft)
	{
		isMovingLeft = false;
		MovingRight = GetTickCount64();
	}

	if (GetTickCount64() - MovingRight > 1500 && !isMovingLeft) {
		isMovingLeft = true;
		MovingLeft = GetTickCount64();
	}

	if (isMovingLeft) {
		vx = -0.02f;
	}
	else
		vx = 0.02f;
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

