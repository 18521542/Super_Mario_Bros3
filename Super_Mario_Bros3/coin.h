#pragma once
#include "GameObject.h"
#define COIN_ANI 0
#define BBCOIN_WIDTH	8
#define BBCOIN_HEIGT	8
class CCoin : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};