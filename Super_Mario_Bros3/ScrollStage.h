#pragma once
#include "GameObject.h"

class ScrollStage : public CGameObject {
public:
	ScrollStage();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom){}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};