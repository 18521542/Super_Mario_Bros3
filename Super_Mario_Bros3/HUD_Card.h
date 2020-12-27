#pragma once
#include "Game.h"
#include "GameObject.h"

#define ANI_PLANT		0
#define ANI_MUSHROOM	1
#define ANI_STAR		2


class HUD_Card : public CGameObject {
	int id;
	bool isAppear = false;
	bool isAllowToAppear = true;
	ULONGLONG	StartAppearTime;
public:
	HUD_Card(int id);
	void StartAppear() { StartAppearTime = GetTickCount64(); isAllowToAppear = false; }
	bool IsAllowToAppear() { return this->isAllowToAppear; }
	virtual void Render();

	void SetID(int id) { this->id = id; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void GetBoundingBox(float& l, float& t, float& r, float& b){}

	void SetIsAppear(bool appear) { this->isAppear = appear; }
};