#pragma once
#include "GameObject.h"

class Timer :public CGameObject 
{
	float value;
public:
	Timer();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
};