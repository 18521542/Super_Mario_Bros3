#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_DIE_SPEED 0.15f;

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 27

#define KOOPAS_BBOX_HEIGHT_DIE 16
#define KOOPAS_BBOX_WIDTH_DIE 16

#define Pushback 20

#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DIE_AND_MOVING 300
#define KOOPAS_STATE_DIE_MOVING	400

#define KOOPAS_ANI_WALKING_RIGHT 0
#define KOOPAS_ANI_WALKING_LEFT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_DIE_MOVING 3

class CKoopas : public CGameObject
{
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	CKoopas();
	virtual void SetState(int state);
};