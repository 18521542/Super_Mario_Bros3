#pragma once
#include "GameObject.h"
#define ANI_SET_FOR_STACK	500000

#define TYPE_WHITE			1
#define TYPE_BLACK			2

#define ANI_POS_FOR_WHITE	0
#define ANI_POS_FOR_BLACK	1

class Stack : public CGameObject
{
	int type;
	float StartSpeed;
	float EndSpeed;
public:
	Stack(int typ,float start, float end );
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};