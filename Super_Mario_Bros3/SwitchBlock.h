#pragma once
#include "Object.h"


class CSwitchBlock : public CGameObject
{
	//virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	//virtual void Render();
	DWORD StartEffectTime = 0;
	bool isUsed = false;
public:
	//CSwitchBlock();
	CSwitchBlock(int state);
	//virtual void SetState(int state);
	//void StartEffect() { StartEffectTime = GetTickCount(); state = COIN_STATE_EFFECT; vy = -0.3f; }
};
