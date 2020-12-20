#pragma once
#include"GameObject.h"

class Turle : public CGameObject {
	ULONGLONG MovingLeft;
	ULONGLONG MovingRight;
	bool isMovingLeft;
public:
	Turle();

	virtual void Render();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void GetBoundingBox(float& l, float& t, float& r, float& b);
};