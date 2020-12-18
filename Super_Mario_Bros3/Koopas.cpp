#include "Koopas.h"
#include "Utils.h"
CKoopas::CKoopas(int type)
{
	this->type = type;
	nx = 1;
	SetState(KOOPAS_STATE_WALKING);
}



void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vy += KP_GRAVITY * dt;

	if (state == KOOPAS_STATE_DIE_UP) 
	{
		if (nx > 0) 
		{
			if (vx > 0)
				vx -= DECREASE_ACCELERATION_SPEED * dt;
			else
				vx = 0;
		}
		else
		{
			if (vx < 0)
				vx += DECREASE_ACCELERATION_SPEED * dt;
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
			if (mario->IsHolding() && this->type != KOOPA_PARATROOPA) 
			{
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
					if (e->ny < 0)
					{
						if (type == KOOPA)
							vy = 0;
						else
							vy = -DEFECT_SPEEDY;
						y += min_ty * dy + ny * 0.4f;
					}
					else {
						x += dx;
						y += dy;
					}
					
				}
				else if (plat->getType() == PLATFORM_TYPE_ONE) 
				{
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					if (e->nx != 0) 
					{	
						this->nx = -this->nx;
						this->vx = this->nx * vx;
					}
					if (ny != 0) 
					{
						if (type == KOOPA)
							vy = 0;
						else
							vy = -DEFECT_SPEEDY;
					}
					
					
				}
				else if (plat->getType() == PLATFORM_TYPE_THREE) {
					if (state == GOOMBA_STATE_WALKING) 
					{
						if (nx != 0)
						{
							this->vx = -this->vx;
							this->nx = -this->nx;
						}
					}
					else {
						x += dx;
						y += dy;
					}
				}
			}
			else if (dynamic_cast<CFireBall*>(e->obj)) {
				CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
				if (nx != 0 || ny != 0) {
					if (this->GetType() != KOOPA)
						this->SetType(KOOPA);
					SetState(KOOPAS_STATE_DIE);
				}
						
				fb->setIsAppear(false);
					
			}	
			else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
				CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(e->obj);
				if (e->nx != 0) 
				{	
					if (state != GOOMBA_STATE_WALKING) {
						if (qb->GetState() == BRICK_STATE_WITH_EFFECT) {
							qb->SetState(BRICK_STATE_WITH_NO_EFFECT);
							if (!qb->IsMoving() && !qb->IsUsed())
							{
								qb->StartMoving();
							}
						}
					}
				}
				x += dx;
				y += dy;
				this->nx = -this->nx;
				vx = this->nx * vx;
			}
			else if (dynamic_cast<CBreakableBrick*>(e->obj)) 
			{
				CBreakableBrick* bb = dynamic_cast<CBreakableBrick*>(e->obj);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (state == KOOPAS_STATE_WALKING) 
				{
					if (e->nx != 0)
					{
						this->nx = -this->nx;
						vx = this->nx * vx;
					}
					if (ny != 0)
					{
						vy = 0;
					}
					
				}
				else if (state == KOOPAS_STATE_DIE_MOVING) {
					if (e->nx!=0) {
						this->nx = -this->nx;
						this->vx = this->nx * this->vx;
						bb->SetState(BREAKABLE_BRICK_STATE_DISAPPEAR);
					}
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) {
				CGoomba* gb = dynamic_cast<CGoomba*>(e->obj);
				gb->SetState(GOOMBA_STATE_DIE);
				gb->StartDisapear();
				x += dx;
				//vy = 0;
				y += dy;
			}
			else if (dynamic_cast<CKoopas*>(e->obj)){
				CKoopas* kp = dynamic_cast<CKoopas*>(e->obj);
				if (e->nx != 0) 
				{
					if (this->GetType() == KOOPA_PARATROOPA) {
						x += dx;
						y += dy;
					}
					else 
					{
						if (this->state == KOOPAS_STATE_DIE_MOVING) {
							if (kp->GetType() != KOOPA) {
								kp->SetType(KOOPA);
							}
							kp->SetState(KOOPAS_STATE_DIE_UP);
							x += dx;
							y += dy;
						}
					}
				}
				if (e->ny != 0 )
				{
					if (this->GetType() == KOOPA_PARATROOPA) {	
							x += dx;
							y += dy;
					}
				}
			}
			else if (dynamic_cast<CMario*>(e->obj)) {
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				CMario* mario = dynamic_cast<CMario*>(e->obj);
				//mario->StartEffect();
			}
		}
	}

	//clean collistion
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
	

void CKoopas::Render()
{
	int ani = KOOPAS_STATE_DIE;
	if (type == KOOPA) 
	{
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_ANI_DIE;
		}
		else if (state == KOOPAS_STATE_WALKING)
		{
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
	}
	else if (type == KOOPA_PARATROOPA) {
		if (nx > 0) {
			ani = KOOPAS_PARATROOPA_ANI_RIGHT;
		}
		else
			ani = KOOPAS_PARATROOPA_ANI_LEFT;
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
		if(isNormal)
			vx = nx * KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_DIE_MOVING:
		if(isNormal)
			vx = nx * KOOPAS_DIE_SPEED;
		break;
	case KOOPAS_STATE_DIE_UP:
		vx =  nx * KOOPAS_WALKING_SPEED;
		vy = -(DEFECT_SPEEDY);
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