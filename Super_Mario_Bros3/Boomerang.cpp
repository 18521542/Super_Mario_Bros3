#include "Boomerang.h"
#include "Utils.h"
#include "PlayScene.h"

#define BB_BOOM_WIDTH	10
#define BB_BOOM_HEIGHT	10

#define TIME_WAITING	150
#define First_SPEED_OF_TRAJECTORY	0.1f
#define TIME_FOR_EACH_TRAJECTORY	700
#define TIME_FOR_WHOLE_MOVING_STATE	2300

#define SPEED_X_GO_AWAY	0.1f
#define SPEED_X_GOBACK1	-0.15f
#define SPEED_X_GOBACK2	-0.14f

#define UPRIGHT_DELTA_SPEED_Y	0.002f
#define DOWNRIGHT_DELTA_SPEED_Y	0.006f
#define DOWNLEFT_DELTA_SPEED_Y	0.0071f
#define UPLEFT_DELTA_SPEED_Y	0.0001f

#define COLLIDE_WITH_MARIO_TIME	200
Boomerang::Boomerang() 
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(BOM_BRO_ANI_SET));
	state = STATE_MOVING;
}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + BB_BOOM_WIDTH;
	bottom = y + BB_BOOM_HEIGHT;
}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	if (isStopUpdate) {
		return;
	}
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (!isCollide) {
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float l, t, r, b;
		mario->GetBoundingBox(l, t, r, b);
		//DebugOut(L"\nTrajectory %d", trajectory);

		if (CheckBB(l, t, r, b) && isAllowCollideOneTime)
		{
			mario->StartEffect();
			if (!isCollide) {
				isCollide = true;
				isAllowCollideOneTime = false;
				TimeCollide = GetTickCount64();
				return;
			}
		}
		if (isWaiting) {
			vx = 0;
			vy = 0;
			isActive = true;
		}

		if (GetTickCount64() - TimeWaitingToMove > TIME_WAITING && isWaiting) {
			isWaiting = false;
			StartMoving();
		}
		if (state == STATE_MOVING) {
			//vx = 0.1f;
			if (!isResetSpeed) {
				vx = First_SPEED_OF_TRAJECTORY;
				vy = 0;
				isResetSpeed = true;
			}

			if (trajectory == UP_RIGHT )
			{
				
				if (GetTickCount64() - MovingUpRight > TIME_FOR_EACH_TRAJECTORY /*+ TimeCollide*/) {
					//isMovingUpRight = false;
					StartMovingDownRight();
				}
			}
			else if (trajectory == DOWN_RIGHT ) {
				if (GetTickCount64() - MovingDownRight > TIME_FOR_EACH_TRAJECTORY /*+ TimeCollide*/) {
					//isMovingDownRight = false;
					StartMovingDownLeft();
				}
			}
			else if (trajectory == DOWN_LEFT ) {
				if (GetTickCount64() - MovingDownLeft > TIME_FOR_EACH_TRAJECTORY/* + TimeCollide*/) {
					//isMovingDownLeft = false;
					StartMovingUpLeft();
				}
			}

			//End Moving
			if (GetTickCount64() - TimeMovingStart > TIME_FOR_WHOLE_MOVING_STATE)
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
			vx = SPEED_X_GO_AWAY;
			vy -= UPRIGHT_DELTA_SPEED_Y;
		}
		else if (trajectory == DOWN_RIGHT) {
			vx = SPEED_X_GO_AWAY;
			vy += DOWNRIGHT_DELTA_SPEED_Y;
		}
		else if (trajectory == DOWN_LEFT) {
			vx = SPEED_X_GOBACK1;
			vy -= DOWNLEFT_DELTA_SPEED_Y;
		}
		else if (trajectory == UP_LEFT) {
			vx = SPEED_X_GOBACK2;
			vy += UPLEFT_DELTA_SPEED_Y;
		}
	}
	else {
		vx = 0;
		vy = 0;
		if (GetTickCount64() - TimeCollide > COLLIDE_WITH_MARIO_TIME) {
			isCollide = false;
			if (trajectory == UP_RIGHT)
			{
				MovingUpRight  = TimeCollide;
			}
			else if (trajectory == DOWN_RIGHT) {
				MovingDownRight = TimeCollide;

			}
			else if (trajectory == DOWN_LEFT) {
				MovingDownLeft = TimeCollide ;
			}
			else if (trajectory == UP_LEFT) {
				MovingUpLeft = TimeCollide;
			}
		}
	}
}
void Boomerang::Render() {
	if (isStopUpdate)
		return;
	if (isActive) {
		if (state != STATE_MOVING)
			animation_set->at(ANI_POS_NOTMOVING)->Render(x, y);
		else
			animation_set->at(ANI_POS_MOVING)->Render(x, y);
	}
}
