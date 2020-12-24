#pragma once
#include "GameObject.h"

class Arrow : public CGameObject {
	int curChoice;
	bool isAllowToSwitch = true;
public:
	Arrow();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b){}
	bool IsAllowToSwitch() { return this->isAllowToSwitch; }
	void SetAllowToSwitch(bool value) { this->isAllowToSwitch = value; }
};
