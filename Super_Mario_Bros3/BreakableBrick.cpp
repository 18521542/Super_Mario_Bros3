#include "BreakableBrick.h"

CBreakableBrick::CBreakableBrick()
{
	SetState(COIN_STATE_APPEAR);
}
void CBreakableBrick::Render()
{
	if(state!=BREAKABLE_BRICK_STATE_DISAPPEAR)
		animation_set->at(0)->Render(x, y);
}

void CBreakableBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state != BREAKABLE_BRICK_STATE_DISAPPEAR) {
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
	else {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if()
}