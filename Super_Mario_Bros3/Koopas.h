#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_DIE_SPEED 0.15f;

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 27

#define KOOPAS_BBOX_HEIGHT_DIE 16


#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DIE_MOVING	400

#define KOOPAS_ANI_WALKING_RIGHT 0
#define KOOPAS_ANI_WALKING_LEFT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_DIE_MOVING 3

class CKoopas : public CGameObject
{
	bool isBeingHold = false;
	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	CKoopas();
	void SetDirection(int dir) { this->nx = dir; }
	virtual void SetState(int state);
	bool IsBeingHold() { return this->isBeingHold; }
	void SetIsBeingHold(bool hold) { this->isBeingHold = hold; }
};