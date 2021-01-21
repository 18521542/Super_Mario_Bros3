#include "coin.h"
#include "Utils.h"
CCoin::CCoin() 
{
	SetState(COIN_STATE_APPEAR);
	vy = 0;
	vx = 0;
	//effect = new Effect();
}

CCoin::CCoin(int state)
{
	SetState(state);
	vy = 0;
	vx = 0;
	//effect = new Effect();
}
void CCoin::Render()
{
	if(state!=COIN_STATE_DISAPPEAR)
		animation_set->at(COIN_ANI)->Render(x, y);
	//effect->Render();
	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state != COIN_STATE_DISAPPEAR) {
		l = x;
		t = y;
		r = x + BBCOIN_WIDTH;
		b = y + BBCOIN_HEIGT;
	}
	else
		l = t = r = b = 0;
}

#define APPEAR_TIME	600
#define APPEAR_TIME_LIMIT	800

#define UP_SPEEAD
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

	if (GetTickCount() - StartEffectTime > APPEAR_TIME && GetTickCount() - StartEffectTime < APPEAR_TIME_LIMIT)
	{
		StartEffectTime = 0;
		state = COIN_STATE_DISAPPEAR;
	}
		
	/*if (isReset) 
	{
		vy -= 0.1f;
		if (GetTickCount64() - StartResetTime > 100) {
			isReset = false;
			isUsed = false;
		}
	}*/
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

	//effect->Update(dt, coObjects);
}

