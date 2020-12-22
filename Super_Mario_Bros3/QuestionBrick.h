#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Object.h"
#include "Effect.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_WITH_EFFECT		1
#define BRICK_STATE_WITH_NO_EFFECT	2

#define QUESTION_ANI_MOVE			0
#define QUESTION_ANI_NOT_MOVE		1

#define MOVING_TIME					400
#define DEFECT_SPEED				-0.1f
#define GRAVITY						0.0005f


class CQuestionBrick : public CGameObject
{
	Effect* effect;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	DWORD TimeStartMove = 0;
	bool isMoving = false;
	bool isUsed = false;
	bool isFinishMoving = false;
	float startY;
	bool isFixed = false;
public:
	CQuestionBrick(float y,float x);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void StartMoving() { TimeStartMove = GetTickCount(); isMoving = true; vy = DEFECT_SPEED; isUsed = true; }
	bool IsMoving() { return this->isMoving; }
	bool IsUsed() { return this->isUsed; }
	bool IsFinishMoving() { return this-isFinishMoving ; }
	float getStartY() { return this->startY; }
	DWORD GetTimeStartMove() { return this->TimeStartMove; }
	bool IsFixed() { return this->isFixed; }
	void SetIsFixed(bool fix) { this->isFixed = fix; }
};