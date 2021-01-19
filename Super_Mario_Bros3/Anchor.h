#pragma once
#include "GameObject.h"

#define ANCHOR_IN	1
#define ANCHOR_OUT	2

class Anchor : public CGameObject {
public:
	Anchor(int stat) { this->state = stat; }
	virtual void Render() { RenderBoundingBox(); }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects){}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) 
	{
		l = x;
		t = y;
		r = x + 10;
		b = y + 10;
	}
};