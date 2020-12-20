#include "TurtleOFWorldMap.h"
#include "Utils.h"

#define TIME_MOVING_BEFORE_CHANGE_DIRECTION		1500
#define	SPEED									0.01f

#define ANI_LEFT		0
#define ANI_RIGHT		1
Turle::Turle() {
	isMovingLeft = true;
	MovingLeft = GetTickCount64();
}

void Turle::Render() {
	int ani;
	if (isMovingLeft) {
		ani = ANI_LEFT;
	}
	else ani = ANI_RIGHT;

	animation_set->at(ani)->Render(x, y);
}

void Turle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);

	x += dx;

	if (GetTickCount64() - MovingLeft > TIME_MOVING_BEFORE_CHANGE_DIRECTION && isMovingLeft)
	{
		isMovingLeft = false;
		MovingRight = GetTickCount64();
	}

	if (GetTickCount64() - MovingRight > TIME_MOVING_BEFORE_CHANGE_DIRECTION && !isMovingLeft) {
		isMovingLeft = true;
		MovingLeft = GetTickCount64();
	}

	if (isMovingLeft) {
		vx = -SPEED;
	}
	else
		vx = SPEED;
}

void Turle::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}