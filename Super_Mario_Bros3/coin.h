#pragma once
#include "GameObject.h"
#define COIN_ANI 0

#define COIN_STATE_DISAPPEAR	1
#define COIN_STATE_APPEAR		2
#define COIN_STATE_EFFECT		3

#define BBCOIN_WIDTH	8
#define BBCOIN_HEIGT	8
class CCoin : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD StartEffectTime = 0;
public:
	CCoin();
	CCoin(int state, int x, int y);
	//virtual void SetState(int state);
	void StartEffect() { StartEffectTime = GetTickCount(); }
};