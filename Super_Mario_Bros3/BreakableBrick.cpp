#include "BreakableBrick.h"

CBreakableBrick::CBreakableBrick(int typ)
{
	SetState(SHINING);
	this->type = typ;
}
void CBreakableBrick::Render()
{
	int ani = 0;

	if (state == COIN) 
	{
		ani = ANI_COIN;
	}
	else if (state == SHINING) 
	{
		ani = ANI_SHINING;
	}
	else if (state == WITHOUT_SHINING) 
	{
		ani = ANI_WITHOUT_SHINING;
	}


	if (state != BREAKABLE_BRICK_STATE_DISAPPEAR) 
	{
		animation_set->at(ani)->Render(x, y);
	}

	//RenderBoundingBox();
}

void CBreakableBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state != BREAKABLE_BRICK_STATE_DISAPPEAR) 
	{
		if (state != COIN) {
			l = x;
			t = y;
			r = x + BRICK_BBOX_WIDTH;
			b = y + BRICK_BBOX_HEIGHT;
		}
		else
		{
			l = x;
			t = y;
			r = x + BBCOIN_WIDTH;
			b = y + BBCOIN_HEIGT;
		}
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
	
}