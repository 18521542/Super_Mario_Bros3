#include "Tail.h"

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
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		float left, top, right, bottom;
		obj->GetBoundingBox(left, top, right, bottom);
		if (dynamic_cast<CMario*>(obj))
		{
			CMario* mario = dynamic_cast<CMario*>(obj);
			if (mario->IsUsingTail())
			{
				this->state = TAIL_STATE_APPEAR;
				this->nx = mario->nx;
			}
			else
				this->state = TAIL_STATE_DISAPPEAR;
			if (mario->nx < 0)
			{
				this->x = left - BB_TAIL_WIDTH;
			}
			else
				this->x = right;
			this->y = (bottom - BB_TAIL_HEIGHT);
		}
		else if (dynamic_cast<CKoopas*>(obj)) 
		{
			float pLeft, pTop, pRight, pBottom;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			if (this->CheckBB(pLeft, pTop, pRight, pBottom)) 
			{
				obj->SetDirection(nx);
				obj->SetState(KOOPAS_STATE_DIE_UP);
			}
		}
	}
}