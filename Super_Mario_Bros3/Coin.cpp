#include "coin.h"
#include "Utils.h"
CCoin::CCoin() 
{
	SetState(COIN_STATE_APPEAR);
	vy = 0;
	vx = 0;
}

CCoin::CCoin(int state)
{
	SetState(state);
	vy = 0;
	vx = 0;
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
	CGameObject::Update(dt,coObjects);
	//do nothing;

	if (isUsed)
	{
		vy += 2*GRAVITY * dt;
	}
	else {
		vy = 0;
	}
	y += dy;

	if (GetTickCount() - StartEffectTime > 600)
	{
		StartEffectTime = 0;
		state = COIN_STATE_DISAPPEAR;
	}
		

	for (size_t i = 0 ; i < coObjects->size(); i++) 
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CQuestionBrick*>(obj)) 
		{
			float qX, qY;
			CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(obj);
			qb->GetPosition(qX, qY);
			if (qb->IsUsed()&& this->x==qX && !this->isUsed) 
			{
				StartEffect();
				isUsed = true;
			}
		}
	}
}

