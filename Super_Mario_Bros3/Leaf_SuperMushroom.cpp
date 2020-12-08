#include "Leaf_SuperMushroom.h"
#include "Utils.h"


CLeaf_Mushroom::CLeaf_Mushroom(int _type)
{
	type = _type;
	vy = 0;
	vx = 0;
	state = LEAF_STATE_RIGHT;
	nx = 1;
}
void CLeaf_Mushroom::Render()
{
	if (isAllowToAppear) 
	{
		if (type == MUSHROOM)
		{
			animation_set->at(MUSHROOM_ANI)->Render(x, y);
		}
		else if (type == LEAF)
		{
			if (state == LEAF_STATE_LEFT)
				animation_set->at(LEAF_ANI_LEFT)->Render(x, y);
			else if (state == LEAF_STATE_RIGHT)
				animation_set->at(LEAF_ANI_RIGHT)->Render(x, y);
		}
	}	
}

void CLeaf_Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (type == LEAF) {
		l = x;
		t = y;
		r = x + LEAF_WIDTH;
		b = y + LEAF_HEIGHT;
	}
	else if (type == MUSHROOM) {
		l = x;
		t = y;
		r = x + MUSHROOM_WIDTH;
		b = y + MUSHROOM_HEIGHT;
	}
}

void CLeaf_Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	//do nothing;
	for (size_t i = 0; i < coObjects->size(); i++) 
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CMario*>(obj)) 
		{
			CMario* mario = dynamic_cast<CMario*>(obj);
			if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				this->type = MUSHROOM;
			}
			else
				this->type = LEAF;
		}
		else if (dynamic_cast<CQuestionBrick*>(obj))
		{
			float qX, qY;
			CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(obj);
			qb->GetPosition(qX, qY);
			if (qb->IsUsed() && this->x == qX && !this->isUsed)
			{
				isUsed = true;
				if (this->type == MUSHROOM)
				{
					MushroomStartMoving();
				}
				else {
					LeafStartMoving();
				}
				isAllowToAppear = true;
				
			}
		}
	}

	y += dy;
	x += dx;

	if (isMushroomMoving) 
	{
		if (GetTickCount() - StartEffectTime < 800) {
			vy += -0.00005f * dt;
		}
		else
		{
			vy += GRAVITY * dt;
			vx = 0.03f;
		}
	}
	else if (isLeafMoving) 
	{
		if (GetTickCount() - StartEffectTime < 100) {
			vy = -0.3f;
		}
		else
		{
			vy = 0.02f;
			vx += nx * 0.0003f * dt;
			if (abs(vx) > 0.1f) 
			{
				nx = -nx;
			}
		}
	}

	if (type == LEAF) 
	{
		if (nx > 0) 
		{
			SetState(LEAF_STATE_RIGHT);
		}
		else if(nx < 0)
			SetState(LEAF_STATE_LEFT);
	}
	
}


