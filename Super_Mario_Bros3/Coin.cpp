#include "coin.h"

CCoin::CCoin() 
{
	SetState(COIN_STATE_APPEAR);
}

CCoin::CCoin(int state, int X,int Y)
{
	SetState(state);
	this->x = X;
	this->y = Y;
}
void CCoin::Render()
{
	if(state!=COIN_STATE_DISAPPEAR)
		animation_set->at(COIN_ANI)->Render(x, y);
	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BBCOIN_WIDTH;
	b = y + BBCOIN_HEIGT;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//do nothing;
	if (state == COIN_STATE_EFFECT) 
	{
		vy += 0.1f;
		if (GetTickCount() - StartEffectTime > 300) 
		{
			StartEffectTime = 0;
			state = COIN_STATE_DISAPPEAR;
		}
	}
}

