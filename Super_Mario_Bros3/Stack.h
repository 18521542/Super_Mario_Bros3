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
	int LevelInStackBar;
	float StartSpeed;
	float EndSpeed;
public:
	Stack(int typ,int level );
	void SetType(int typ) { this->type = typ; }
	int GetType() { return this->type; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, int level);
	virtual void Render();
	int GetLevel() { return this->LevelInStackBar; }
};