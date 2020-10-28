#include "Koopas.h"
#include "Platform.h"
#include "Mario.h"
#include "Utils.h"
CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (IsBeingHold()) 
	{
		for (int i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			float pLeft, pTop, pRight, pBottom;
			float xM, yM, nM;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			if (dynamic_cast<CMario*>(obj))
			{
				obj->GetPosition(xM, yM);
				nM = obj->GetDirection();
				if (CheckBB(pLeft, pTop, pRight, pBottom))
				{
					x = xM + nM * KOOPAS_BBOX_WIDTH;
					y = yM-KOOPAS_BBOX_HEIGHT_DIE;
				}
			}

		}
		
	}
	else 
	{
		CGameObject::Update(dt, coObjects);

		vy += MARIO_GRAVITY * dt;

		//Push back a little bit when bounding box has collision
		for (int i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			float pLeft, pTop, pRight, pBottom;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			if (dynamic_cast<CPlatform*>(obj))
				if (CheckBB(pLeft, pTop, pRight, pBottom))
				{
					y -= y + KOOPAS_BBOX_HEIGHT - pTop + PushBackPixel;
					x += -nx * (x + KOOPAS_BBOX_WIDTH - pTop + PushBackPixel);
				}
		}
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		
		CalcPotentialCollisions(coObjects, coEvents);


		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CPlatform*>(e->obj))
				{
					CPlatform* plat = dynamic_cast<CPlatform*>(e->obj);
					if (plat->getType() == PLATFORM_TYPE_TWO) {
						if (e->ny < 0)
							SetState(KOOPAS_STATE_WALKING);
					}
					else if (plat->getType() == PLATFORM_TYPE_ONE) {
						// block every none-special object 
						x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;

						if (nx != 0) {
							vx = -vx;
							this->nx = -this->nx;
						}

						if (ny != 0)
							vy = 0;
					}
				}
				if (dynamic_cast<CMario*>(e->obj))
				{
					if(isBeingHold)
					{
						if (nx == 0)
							SetIsBeingHold(false);
						DebugOut(L"\nkoopas is being hold is %d", IsBeingHold());
					}
					
				}
			}
		}
}
	//}
}

void CKoopas::Render()
{
	int ani = KOOPAS_STATE_DIE;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_WALKING) {
		if (nx > 0) 
			ani = KOOPAS_ANI_WALKING_RIGHT;
		else  ani = KOOPAS_ANI_WALKING_LEFT;
	}
	else if (state == KOOPAS_STATE_DIE_MOVING) {
		ani = KOOPAS_ANI_DIE_MOVING;
	}
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = nx * KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_DIE_MOVING:
		vx = nx * KOOPAS_DIE_SPEED;
		break;
	}

}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	
	if (state == KOOPAS_STATE_DIE || state==KOOPAS_STATE_DIE_MOVING) 
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else 
		bottom =  y + KOOPAS_BBOX_HEIGHT;
	;
}