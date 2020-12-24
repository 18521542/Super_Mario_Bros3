#pragma once
#include "GameObject.h"
#define ANI_SET_NUMBER	100000

class Number :public CGameObject {
	int value = 0;
public:
	Number(int val);
	void SetValue(int val) { this->value = val; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
};
