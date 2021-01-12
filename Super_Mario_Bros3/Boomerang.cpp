#include "Boomerang.h"
#include "Utils.h"

Boomerang::Boomerang() 
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(BOM_BRO_ANI_SET));
	state = STATE_MOVING;
}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom) {

}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	CGameObject::Update(dt, coObjects);
	
	x += dx;
	y += dy;

	if (isWaiting) {
		vx = 0;
		vy = 0;
		isActive = true;
	}

	if (GetTickCount64() - TimeWaitingToMove > 150 && isWaiting) {
		isWaiting = false;
		StartMoving();
	}
	if (state == STATE_MOVING) {
		
		//vx = 0.1f;
		if (!isResetSpeed) {
			vx = 0.1f;
			vy = 0;
			isResetSpeed = true;
		}
			
		if (trajectory == UP_RIGHT) 
		{
			if (GetTickCount64() - MovingUpRight > 700) {
				//isMovingUpRight = false;
				StartMovingDownRight();
			}
		}
		else if (trajectory == DOWN_RIGHT) {
			if (GetTickCount64() - MovingDownRight > 700) {
				//isMovingDownRight = false;
				StartMovingDownLeft();
			}
		}
		else if (trajectory == DOWN_LEFT) {
			if (GetTickCount64() - MovingDownLeft > 700) {
				//isMovingDownLeft = false;
				StartMovingUpLeft();
			}
		}


		//End Moving
		if (GetTickCount64() - TimeMovingStart > 2300) 
		{
			SetState(STATE_NOT_MOVING);
			isMoving = false;
			isActive = false;
			vx = 0;
			vy = 0;
			isResetSpeed = false;
		}
	}

	if (trajectory == UP_RIGHT) {
		vx = 0.1f;
		vy -= 0.002f;
	}
	else if (trajectory == DOWN_RIGHT) {
		vx = 0.1f;
		vy += 0.006f;
	}
	else if (trajectory == DOWN_LEFT) {
		vx = -0.15f;
		vy -= 0.0071f;
	}
	else if (trajectory == UP_LEFT) {
		vx = -0.14f;
		vy += 0.0001f;
	}
}
void Boomerang::Render() {
	if (isActive) {
		if (state != STATE_MOVING)
			animation_set->at(ANI_POS_NOTMOVING)->Render(x, y);
		else
			animation_set->at(ANI_POS_MOVING)->Render(x, y);
	}
}
