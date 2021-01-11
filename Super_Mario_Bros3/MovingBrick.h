#pragma once
#include "GameObject.h"
#define MovingBrickWidth	62
#define MovingBrickHeight	22
#define STATE_BRICK_MOVING	1
#define STATE_BRICK_FALLING	2

class MovingBrick : public CGameObject {
public:
	MovingBrick();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int stat) { this->state = stat; }
};