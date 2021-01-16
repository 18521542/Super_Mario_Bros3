#pragma once
#include "GameObject.h"
#define SCREEN__WIDTH 330
#define SCREEN__HEIGHT 290

class MovingEdge : public CGameObject {
protected:
	float  StopDes;
	bool isActive = true;
public :
	bool IsActive() { return this->isActive; }
	MovingEdge(float stopDes);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};