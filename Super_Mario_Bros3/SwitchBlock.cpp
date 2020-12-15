#include "SwitchBlock.h"

CSwitchBlock::CSwitchBlock(int stat) 
{
	this->state = stat;
	this->isAppear = false;
}

void CSwitchBlock::Render()
{
	int ani = 0;
	if (state == STATE_NEW)
		ani = ANI_NEW;
	else
		ani = ANI_IS_ACTIVATED;

	if (isAppear) 
	{
		animation_set->at(ani)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CSwitchBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isAppear) 
	{
		if (state == STATE_NEW)
		{
			l = x;
			t = y;
			r = x + 16;
			b = y + 16;
		}
		else
		{
			l = 0;
			t = 0;
			r = 0;
			b = 0;
		}
	}
	else
	{
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
}

void CSwitchBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//do nothing;

	for (size_t i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBreakableBrick*>(obj)) 
		{
			CBreakableBrick* bb = dynamic_cast<CBreakableBrick*>(obj);
			if (bb->GetType() == SPECIAL) {
				if (bb->GetState() == WITHOUT_SHINING) {
					this->isAppear = true;
				}
			}
			if (this->state != STATE_NEW) 
			{
				if (bb->GetState() == SHINING) 
				{
					bb->SetState(COIN);
				}
				else if (bb->GetState() == WITHOUT_SHINING) {
					this->y = bb->y - HEIGHT_ACTIVATED;
				}
			}
		}
	}
}

void CSwitchBlock::SetState(int state) {
	this->state = state;
}