#pragma once
#include "GameObject.h"
#include "Object.h"
#define TAIL_ANI 0
#define TAIL_STATE_DISAPPEAR	1
#define TAIL_STATE_APPEAR		2

#define BB_TAIL_WIDTH			8
#define BB_TAIL_HEIGHT			8

#define TAIL_APPEAR_TIME		10

class CTail : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD startAppear;
	bool isAppear = false;
public:
	void StartAppear() { startAppear = GetTickCount(); isAppear = true; }
	CTail();
	//virtual void SetState(int state);
}; 
