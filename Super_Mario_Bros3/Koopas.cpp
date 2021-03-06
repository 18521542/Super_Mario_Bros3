#include "Koopas.h"
#include "Utils.h"
#include "PlayScene.h"

#define KOOPA_1_4_FLY_SPEED	0.1f
#define HALF_LIMIT_DISTANCE	50
CKoopas::CKoopas(int type)
{
	this->type = type;
	nx = 1;
	SetState(KOOPAS_STATE_WALKING);
	if (type == JUST_FLY_KOOPA) {
		vy = KOOPA_1_4_FLY_SPEED;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (type != JUST_FLY_KOOPA)
	vy += KP_GRAVITY * dt;

	if (type == JUST_FLY_KOOPA) 
	{
		nx = -1;
		vx = 0;
		//vy = 0.1f;
		

		float LimitUp = StartY - HALF_LIMIT_DISTANCE;
		float LimitDown = StartY + HALF_LIMIT_DISTANCE;

		if ((y > LimitDown&& IsDown) ) {
			IsDown = false;
			vy = -vy;
		}
		
		if ((y < LimitUp && !IsDown)) {
			IsDown = true;
			vy = -vy;
		}
		CGameObject::Update(dt, coObjects);
		y += dy;
		return;
	}
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
	
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsHolding() && this->type != KOOPA_PARATROOPA)
	{
		vy = 0;
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
				float l, t, r, b;
				plat->GetBoundingBox(l, t, r, b);
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
						this->vx = -this->vx;
						this->nx = -this->nx;
						if (e->ny != 0) {
							vy = 0;
						}
					}
					if (e->ny != 0)
					{
						if (type == KOOPA)
							vy = 0;
						else
							vy = -DEFECT_SPEEDY;
						/*y += min_ty * dy + ny * 0.4f;*/
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
						if (e->ny != 0) {
							vy = 0;
							y += min_ty * dy + ny * 0.4f;
						}
						if (bb->IsMoving()) {
							SetState(KOOPAS_STATE_DIE_UP);
						}
					}
					if (e->ny != 0)
					{
						vy = 0;
						//x += min_tx * dx + nx * 0.4f;
						
					}
					
				}
				else if (state == KOOPAS_STATE_DIE_MOVING) 
				{
					if (e->nx!=0) 
					{
						this->nx = -this->nx;
						this->vx = this->nx * this->vx;
						bb->SetState(BREAKABLE_BRICK_STATE_DISAPPEAR);
					}
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) {
				CGoomba* gb = dynamic_cast<CGoomba*>(e->obj);
				gb->SetSpeed(0, GGOMBA_DEFLECT_SPEED);
				gb->SetKillByKoopa(true);
				x += dx;
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
			}
			else if (dynamic_cast<MovingBrick*>(e->obj)) {
				x += dx;
				y += dy;
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
	else if (type == KOOPA_PARATROOPA || type == JUST_FLY_KOOPA) {
		if (nx > 0) {
			ani = KOOPAS_PARATROOPA_ANI_RIGHT;
		}
		else
			ani = KOOPAS_PARATROOPA_ANI_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
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