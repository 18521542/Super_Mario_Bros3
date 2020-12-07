#include "Koopas.h"
#include "Utils.h"
CKoopas::CKoopas()
{
	nx = 1;
	SetState(KOOPAS_STATE_WALKING);
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

		CGameObject::Update(dt, coObjects);

		vy += MARIO_GRAVITY * dt;

		if (state == KOOPAS_STATE_DIE_UP) 
		{
			if (nx > 0) 
			{
				if (vx > 0)
					vx -= 0.0002f * dt;
				else
					vx = 0;
			}
			else
			{
				if (vx < 0)
					vx += 0.0002f * dt;
				else
					vx = 0;
			}
		}
		
		for (size_t i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			float pLeft, pTop, pRight, pBottom;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			
			if (dynamic_cast<CMario*>(obj)) {
				CMario* mario = dynamic_cast<CMario*>(obj);
				if (mario->IsHolding()) {
					vy = 0;
				}
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
						x += dx;
						y += dy;
					}
					else if (plat->getType() == PLATFORM_TYPE_ONE) 
					{
						if (nx != 0) 
						{	
							float l, t, r, b;
							plat->GetBoundingBox(l, t, r, b);
							vx = -vx;
							this->nx = -this->nx;
							x += min_tx * dx + nx * 0.4f;
							y += min_ty * dy + ny * 0.4f;

							//if overlap occur
							if (this->CheckBB(l, t, r, b))
							{
								if (this->nx > 0) {
									x = x - 1.0f;
								}
								else
									x = x + 1.0f;
							}
						}
						if (ny != 0) 
						{
							x += min_tx * dx + nx * 0.4f;
							y += min_ty * dy + ny * 0.4f;
							vy = 0;
						}
					}
				}
				else if (dynamic_cast<CFireBall*>(e->obj)) {
					CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
					if (nx != 0 || ny != 0) {
						SetState(KOOPAS_STATE_DIE);
					}
						
					fb->setIsAppear(false);
					
				}	
				else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
					CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);
					if (e->nx != 0) {
						if (qb->GetState() == BRICK_STATE_WITH_EFFECT) {
							qb->SetState(BRICK_STATE_WITH_NO_EFFECT);
							if (!qb->IsMoving() && !qb->IsUsed())
							{
								qb->StartMoving();
							}
						}
					}
				}
			}
		}


		//clean collistion
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
	else if (state == KOOPAS_STATE_DIE_UP) {
		ani = KOOPAS_ANI_DIE_up;
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
	case KOOPAS_STATE_DIE_UP:
		vx =  nx * KOOPAS_DIE_SPEED;
		vy = -(0.3f);
	}

}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE || state == KOOPAS_STATE_DIE_MOVING|| state == KOOPAS_STATE_DIE_UP)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else 
		bottom =  y + KOOPAS_BBOX_HEIGHT;
	;
}