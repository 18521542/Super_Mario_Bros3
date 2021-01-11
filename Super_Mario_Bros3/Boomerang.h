#pragma once
#include "GameObject.h"
#define BOM_BRO_ANI_SET			129101
class Boomerang : public CGameObject {
public :
	Boomerang();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int stat) { this->state = stat; }
};