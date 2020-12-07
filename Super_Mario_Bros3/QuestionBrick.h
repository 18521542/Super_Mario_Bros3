#pragma once
#include "GameObject.h"
#include "Utils.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_WITH_EFFECT		1
#define BRICK_STATE_WITH_NO_EFFECT	2

#define MOVING_TIME 20


class CQuestionBrick : public CGameObject
{
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	DWORD TimeStartMove = 0;
	bool isMoving = false;
	bool isUsed = false;
public:
	CQuestionBrick();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void StartMoving() { TimeStartMove = GetTickCount(); isMoving = true; vy = -0.1f; isUsed = true; }
	bool IsMoving() { return this->isMoving; }
	bool IsUsed() { return this->isUsed; }
};