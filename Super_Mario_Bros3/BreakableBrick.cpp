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
	else
		pieceANI->Render();
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

#define TIME_MOVING	100
#define TIME_FOR_COIN_APPEAR	4500
void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (size_t i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		float pLeft, pTop, pRight, pBottom;
		obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
		if (dynamic_cast<CKoopas*>(obj))
		{
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			CKoopas* kp = dynamic_cast<CKoopas*>(obj);
			if (CheckBB(pLeft, pTop, pRight, pBottom) && kp->GetState() != KOOPAS_STATE_DIE_UP)
			{
				kp->SetState(KOOPAS_STATE_DIE_UP);
			}
		}
	}

	if (isMoving) {
		CGameObject::Update(dt, coObjects);
		y += dy;
		if (GetTickCount64() - TimeStartMove > TIME_MOVING) {
			TimeStartMove = 0;
			isMoving = false;
			y = startY;
		}
		return;
	}
	if (!isInitPieceANI)
	{
		pieceANI = new PiecesANI(this->x, this->y);
		isInitPieceANI = true;
		SetStartPos(this->x, this->y);
	}

	if (this->state == COIN)
	{
		isNotAllowAniBreakAppear = true;
		if (GetTickCount64() - CoinAppearTime > TIME_FOR_COIN_APPEAR && !isActivated)
		{
			//this->state = SHINING;		
			this->SetState(SHINING);
			CoinAppearTime = 0;
			isActivated = true;
			isNotAllowAniBreakAppear = false;
		}
	}

	if (state == BREAKABLE_BRICK_STATE_DISAPPEAR && !isNotAllowAniBreakAppear)
	{
		pieceANI->StartAppear();
		pieceANI->Update(dt, coObjects);
	}
	
}