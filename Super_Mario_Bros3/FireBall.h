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

#define FB_OF_VENUS			1
#define FB_OF_VENUS_SPEED	0.05f
#define TO_CENTER			5

class CFireBall : public CGameObject 
{
	
	int type;
	ULONGLONG startFly;
	bool isAppear = false;
	bool isHolded = true;
	
	
public:
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//virtual void SetState(int state);
	CFireBall(float x,float y);
	CFireBall();
	CFireBall(int typ);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void StartAppear() { isAppear = true; startFly = GetTickCount64(); isHolded = false; }

	bool IsAppear() { return this->isAppear; }
	bool IsHolded() { return this->isHolded; }
	void setIsAppear(bool appear){  this->isAppear = appear; }
	void setIsHolded(bool holded) { this->isHolded = holded; }

	bool GetType() { return this->type; }
	void SetType(int typ) { this->type = typ; }
	void SetDir(int DirX, int DirY) {
		this->nx = DirX;
		this->ny = DirY;
	}
	int ny;
	~CFireBall();
};