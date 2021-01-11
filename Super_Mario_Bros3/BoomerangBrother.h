#pragma once
#include "GameObject.h"
#include "Boomerang.h"

class BoomerangBrother : public CGameObject 
{
	Boomerang* FirstWeapon;
	Boomerang* SecondWeapon;
	bool isInitedWeapon = false;
public:
	BoomerangBrother();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int stat) { this->state = stat; }
};