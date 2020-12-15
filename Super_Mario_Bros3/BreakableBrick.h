#pragma once
#include"Object.h"

#define BREAKABLE_BRICK_STATE_DISAPPEAR 1

#define NORMAL							1
#define SPECIAL							2

#define COIN							6
#define SHINING							2
#define	WITHOUT_SHINING					3

#define ANI_COIN						1
#define ANI_WITHOUT_SHINING				2
#define ANI_SHINING						0

class CBreakableBrick : public CGameObject 
{
	int type;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	bool isBeingBroken = false;
	DWORD StartBroken;
public:
	CBreakableBrick(int typ);
	int GetType() { return this->type; }
};