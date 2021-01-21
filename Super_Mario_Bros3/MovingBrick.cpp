#include "MovingBrick.h"
#include "MovingEdge.h"
#include "PlayScene.h"

#define MOVING_BRICK_SPEED	-0.03f
#define ACCELERATION_OF_MVB	0.0001f

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
		vx = MOVING_BRICK_SPEED;
		vy = 0;
	}
	else if (state == STATE_BRICK_FALLING) {
		vx = 0;
		vy += ACCELERATION_OF_MVB *dt;
	}
}
void MovingBrick::Render() {
	int ani = -1;
	animation_set->at(0)->Render(x, y);
}