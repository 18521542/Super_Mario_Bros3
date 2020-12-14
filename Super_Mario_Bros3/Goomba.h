#pragma once
#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH				16
#define GOOMBA_BBOX_HEIGHT				15
#define GOOMBA_BBOX_HEIGHT_DIE			9

#define GOOMBA_STATE_WALKING			100
#define GOOMBA_STATE_DIE				200

#define GOOMBA_ANI_WALKING				0
#define GOOMBA_ANI_DIE					1

#define PARA_GOOMBA_BB_WIDTH			20
#define PARA_GOOMBA_BB_HEIGHT			19
#define PARA_GOOMBA_BB_FLY_HEIGHT		24

#define PARA_GOOMBA_ANI_FLY				4
#define PARA_GOOMBA_ANI_WALKING			3
#define PARA_GOOMBA_ANI_READY_TO_FLY	2

#define PARA_GOOMBA_STATE_FLY			300
#define PARA_GOOMBA_STATE_WALKING		400
#define PARA_GOOMA_STATE_READY_TO_FLY	500

#define GOOMBA							1
#define PARA_GOOMBA						2

class CGoomba : public CGameObject
{
	int type;
	bool isDisapear = false;
	bool isWalking = true;
	bool isSwitched = false;
	ULONGLONG TimeStartDisappear;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	ULONGLONG TimeFly;
	ULONGLONG TimeReadyToFly;
	ULONGLONG TimeWalking;
	ULONGLONG TimeToSwitch;
public:
	CGoomba(int type);
	void SetType(int type) { this->type = type; }
	int GetType() { return this->type; }
	virtual void SetState(int state);
	void StartDisapear() { TimeStartDisappear = GetTickCount64(); }
	void SetSwitched(bool switched) { this->isSwitched = switched; TimeToSwitch = GetTickCount64(); }
	bool IsSwitched() { return this->isSwitched; }
	void StartReady() { TimeReadyToFly = GetTickCount64(); state = PARA_GOOMA_STATE_READY_TO_FLY; }
	void StartFly() { TimeFly = GetTickCount64(); state = PARA_GOOMBA_STATE_FLY; }
	void StartWalking() { TimeWalking = GetTickCount64(); state = PARA_GOOMBA_STATE_WALKING; }
};