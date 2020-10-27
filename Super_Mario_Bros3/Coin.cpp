#include "coin.h"

CCoin::CCoin() 
{
	
	SetState(COIN_STATE_APPEAR);
}
void CCoin::Render()
{
	if(state==COIN_STATE_APPEAR)
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
}
