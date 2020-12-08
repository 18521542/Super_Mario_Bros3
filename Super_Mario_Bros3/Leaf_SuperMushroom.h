#pragma once
#include "Object.h"

#define MUSHROOM	1
#define LEAF		2

#define LEAF_STATE_LEFT		3
#define LEAF_STATE_RIGHT	4

#define LEAF_ANI_LEFT	0
#define LEAF_ANI_RIGHT	1

#define LEAF_WIDTH		16
#define LEAF_HEIGHT		16

#define MUSHROOM_WIDTH	16
#define MUSHROOM_HEIGHT	16

#define TIME_PHASE_ONE	100
#define MUSHROOM_ANI	2
class CLeaf_Mushroom : public CGameObject
{
	int type;
	bool isAllowToAppear = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD StartEffectTime = 0;
	bool isLeafMoving = false;
	bool isMushroomMoving = false;
	bool isUsed = false;
public:
	CLeaf_Mushroom(int state);
	void LeafStartMoving() { StartEffectTime = GetTickCount(); isLeafMoving = true; };
	void MushroomStartMoving() { StartEffectTime = GetTickCount(); isMushroomMoving = true; };
	//virtual void SetState(int state);
};