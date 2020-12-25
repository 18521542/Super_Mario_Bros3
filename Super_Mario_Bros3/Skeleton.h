#pragma once
#include "GameObject.h"

#define ANI_SET_FOR_SKELETON	200300
#define CARD_SKELETON_POS		1
#define MAIN_SKELETON_POS		0

#define MAIN_SKELETON	1
#define CARD_SKELETON	2

class Skeleton : public CGameObject 
{
	int type;
public:
	Skeleton(int typ);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom){}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){}
	virtual void Render();
};