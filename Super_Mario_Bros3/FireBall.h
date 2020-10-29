#pragma once
#include"GameObject.h"

#define FB_STATE_FLY		100
#define FB_FLY_TIME			3000

#define FB_BBOX_WIDTH		8
#define FB_BBOX_HEIGHT		9

#define FB_SPEED_X			0.015f
#define FB_SPEED_Y			0.015f

#define FB_ANI				0

class CFireBall : public CGameObject 
{

	DWORD startFly;
	bool isAppear = false;
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//virtual void SetState(int state);
	//CFireBall(float x,float y);
	CFireBall();
	//void StartAppear();
};