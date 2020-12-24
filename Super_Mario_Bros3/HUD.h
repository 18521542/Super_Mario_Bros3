#pragma once
#include "GameObject.h"
class HUD : public CGameObject {
	static HUD* __instance;
	int marioSpeed;
	float Time;
	vector<int> score;
public:
	static HUD* getInstance() 
	{ 
		if (__instance == NULL) 
			__instance = new HUD();
		return __instance;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};