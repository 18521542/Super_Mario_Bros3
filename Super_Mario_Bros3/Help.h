#pragma once
#include"GameObject.h"

class Help : public CGameObject {
	ULONGLONG isAppearTime;
	ULONGLONG isNotAppearTime;
	bool isAppear;
public:
	Help();

	virtual void Render();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void GetBoundingBox(float& l, float& t, float& r, float& b);
};