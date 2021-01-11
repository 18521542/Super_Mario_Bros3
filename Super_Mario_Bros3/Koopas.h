#pragma once

#include "GameObject.h"
#include "Object.h"

#define KOOPAS_WALKING_SPEED			0.03f
#define KOOPAS_DIE_SPEED				0.18f
#define DECREASE_ACCELERATION_SPEED		0.00002f
#define DEFECT_SPEEDY					0.18f
#define KP_GRAVITY						0.0005f

#define KOOPAS_BBOX_WIDTH			16
#define KOOPAS_BBOX_HEIGHT			25

#define KOOPAS_BBOX_HEIGHT_DIE		16

#define SAFETY_DISTANCE_TO_HOLD					3.0f

#define SAFETY_HEIGHT_FOR_SMALL_TO_HOLD			1.0f
#define SAFETY_HEIGHT_FOR_BIG_TO_HOLD			10.0f


#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_DIE			200
#define KOOPAS_STATE_DIE_MOVING		400
#define KOOPAS_STATE_DIE_UP			500

#define KOOPAS_ANI_WALKING_RIGHT	0
#define KOOPAS_ANI_WALKING_LEFT		1
#define KOOPAS_ANI_DIE				2
#define KOOPAS_ANI_DIE_MOVING		3
#define KOOPAS_ANI_DIE_up			4

#define KOOPAS_PARATROOPA_ANI_LEFT	5
#define KOOPAS_PARATROOPA_ANI_RIGHT	6

#define KOOPA_PARATROOPA			2
#define KOOPA						1

class CKoopas : public CGameObject
{
	bool isBeingHold = false;
	bool isNormal = true;
	int type;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	CKoopas(int type);
	void SetDirection(int dir) { this->nx = dir; }
	virtual void SetState(int state);
	int GetType() { return this->type; }
	void SetType(int type) { this->type = type; }
	bool IsBeingHold() { return this->isBeingHold; }
	void SetIsBeingHold(bool hold) { this->isBeingHold = hold; }
};