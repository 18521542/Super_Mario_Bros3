#pragma once
#include "GameObject.h"
#include "Object.h"
#include "Effect.h"
#define COIN_ANI 0

#define COIN_STATE_DISAPPEAR	1
#define COIN_STATE_APPEAR		2
#define COIN_STATE_EFFECT		3

#define BBCOIN_WIDTH	8
#define BBCOIN_HEIGT	8
class CCoin : public CGameObject
{
	//Effect* effect;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD StartEffectTime = 0;
	bool isUsed = false;
public:
	//E/ffect* GetEffect() { return this->effect; }
	CCoin();
	CCoin(int state);
	//virtual void SetState(int state);
	void StartEffect() { StartEffectTime = GetTickCount(); state = COIN_STATE_EFFECT; vy = -0.3f; }
};