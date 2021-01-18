#include "Help.h"
#include "Utils.h"

#define TIME_APPEAR	1000

#define ANI		0

Help::Help() {
	isAppear = true;
	isAppearTime = GetTickCount64();
}

void Help::Render() {
	if (isAppear) 
	{
		animation_set->at(ANI)->Render(x, y);
	}	
}

void Help::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);

	if (GetTickCount64() - isAppearTime > TIME_APPEAR && isAppear)
	{
		isAppear = false;
		isNotAppearTime = GetTickCount64();
	}

	if (GetTickCount64() - isNotAppearTime > TIME_APPEAR && !isAppear) {
		isAppear = true;
		isAppearTime = GetTickCount64();
	}
}

void Help::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}
