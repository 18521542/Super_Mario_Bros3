#include "MovingBrick.h"


MovingBrick::MovingBrick() {
	state = STATE_BRICK_MOVING;
	vx = 0;
	vy = 0;
}
void MovingBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + MovingBrickWidth;
	bottom = y + MovingBrickHeight;
}

void MovingBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (state == STATE_BRICK_MOVING) {
		vx = -0.03f;
		vy = 0;
	}
	else if (state == STATE_BRICK_FALLING) {
		vx = 0;
		vy = 0.05f;
	}
}
void MovingBrick::Render() {
	int ani = -1;
	animation_set->at(0)->Render(x, y);
}