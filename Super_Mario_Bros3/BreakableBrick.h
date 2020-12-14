#pragma once
#include"Object.h"

#define BREAKABLE_BRICK_ANI 0

#define BREAKABLE_BRICK_STATE_DISAPPEAR 1


class CBreakableBrick : public CGameObject 
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	bool isBeingBroken = false;
	DWORD StartBroken;
public:
	CBreakableBrick();


};