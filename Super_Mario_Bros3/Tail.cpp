#include "Tail.h"
#include "Utils.h"
#include "PlayScene.h"
CTail::CTail()
{
	SetState(TAIL_STATE_APPEAR);
}
void CTail::Render()
{
	if (state == TAIL_STATE_APPEAR) {
		RenderBoundingBox();
	}
		
}

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == TAIL_STATE_APPEAR) {
		l = x;
		t = y;
		r = x + BB_TAIL_WIDTH;
		b = y + BB_TAIL_HEIGHT;
	}
	else {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"\n Co nhay vo day");

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float left, top, right, bottom;
	mario->GetBoundingBox(left, top, right, bottom);
	if (mario->IsForTailAppear())
	{
		state = TAIL_STATE_APPEAR;
		this->nx = mario->nx;
	}
	else
		state = TAIL_STATE_DISAPPEAR;
	if(mario->GetDirection()<0)
		SetPosition(left - BB_TAIL_WIDTH, bottom - BB_TAIL_HEIGHT);
	else if(mario->GetDirection()>0)
		SetPosition(right , bottom - BB_TAIL_HEIGHT);
	
	for (size_t i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		float left, top, right, bottom;
		obj->GetBoundingBox(left, top, right, bottom);
		if (dynamic_cast<CKoopas*>(obj)) 
		{
			float pLeft, pTop, pRight, pBottom;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			if (this->CheckBB(pLeft, pTop, pRight, pBottom)) 
			{
				obj->SetDirection(nx);
				obj->SetState(KOOPAS_STATE_DIE_UP);
			}
		}
		else if (dynamic_cast<CBreakableBrick*>(obj)) 
		{
			CBreakableBrick* bb = dynamic_cast<CBreakableBrick*>(obj);
			float pLeft, pTop, pRight, pBottom;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			if (this->CheckBB(pLeft, pTop, pRight, pBottom))
			{
				if (bb->GetType() == NORMAL) 
				{
					bb->SetState(BREAKABLE_BRICK_STATE_DISAPPEAR);
				}
				else if (bb->GetType() == SPECIAL) 
				{
					if(bb->GetState()==SHINING)
						bb->SetState(WITHOUT_SHINING);
				}
					
			}
		}
		else if (dynamic_cast <CQuestionBrick*>(obj))
		{
			CQuestionBrick* qb = dynamic_cast <CQuestionBrick*>(obj);

			if (CheckBB(left, top, right, bottom))
			{
				if (qb->GetState() != BRICK_STATE_WITH_NO_EFFECT)
				{
					qb->SetState(BRICK_STATE_WITH_NO_EFFECT);
					if (!qb->IsMoving() && !qb->IsUsed())
					{
						qb->StartMoving();
					}
				}
			}
		}
		else if (dynamic_cast<CGoomba*>(obj)) {
			CGoomba* gb = dynamic_cast<CGoomba*>(obj);
			if (CheckBB(left, top, right, bottom)) {
				gb->SetState(GOOMBA_STATE_DIE);
				gb->StartDisapear();
			}
		}
	}
}