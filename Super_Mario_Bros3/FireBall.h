#pragma once
#include"GameObject.h"

#define FB_STATE_FLY		100
#define FB_APPEAR_TIME		1000

#define FB_BBOX_WIDTH		8
#define FB_BBOX_HEIGHT		9

#define FB_SPEED_X			0.15f
#define FB_SPEED_Y			0.015f

#define UP_SPEED			0.15f
#define FB_ANI				0

class CFireBall : public CGameObject 
{

	ULONGLONG startFly;
	bool isAppear = false;
	bool isHolded = true;
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//virtual void SetState(int state);
	CFireBall(float x,float y);
	CFireBall();
	void StartAppear() { isAppear = true; startFly = GetTickCount64(); isHolded = false; }

	bool IsAppear() { return this->isAppear; }
	bool IsHolded() { return this->isHolded; }
	void setIsAppear(bool appear){  this->isAppear = appear; }
	void setIsHolded(bool holded) { this->isHolded = holded; }
	~CFireBall();
};