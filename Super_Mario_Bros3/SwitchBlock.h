#pragma once
#include "Object.h"

#define STATE_DISAPPEAR			10
#define STATE_IS_ACTIVATED		2
#define STATE_NEW				1

#define ANI_NEW					0
#define ANI_IS_ACTIVATED		1

#define WIDTH					16
#define HEIGHT_NEW				16
#define HEIGHT_ACTIVATED		7

class CSwitchBlock : public CGameObject
{
	bool isAppear = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD StartEffectTime = 0;
	bool isUsed = false;
public:
	void SetState(int state);
	CSwitchBlock(int state);
	//virtual void SetState(int state);
};
