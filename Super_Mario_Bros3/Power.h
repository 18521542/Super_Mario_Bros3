#pragma once
#include "GameObject.h"

#define ANI_SET_FOR_POWER	400000

#define	TYPE_NORMAL			1
#define TYPE_ACTIVATED		2

#define ANI_POS_WHEN_ACTIVATED	2
#define ANI_POS_WHEN_NORMAL		1


class Power : public CGameObject
{
	int type;
public:
	Power(int typ);
	void SetType(int typ) { this->type = typ; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};