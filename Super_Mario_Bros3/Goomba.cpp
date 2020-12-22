#include "Goomba.h"
#include "Mario.h"
#include "Platform.h"
CGoomba::CGoomba(int type)
{
	this->type = type;
	if (type == PARA_GOOMBA) 
	{
		SetState(PARA_GOOMBA_STATE_WALKING);
		StartWalking();
	}
	vx = -GOOMBA_WALKING_SPEED;
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (type == GOOMBA) {
		if (state != GOOMBA_STATE_DIE) {
			left = x;
			top = y;
			right = x + GOOMBA_BBOX_WIDTH;

			if (state == GOOMBA_STATE_DIE)
				bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
			else
				bottom = y + GOOMBA_BBOX_HEIGHT;
		}
		else
		{
			left = 0;
			top = 0;
			right = 0;
			bottom = 0;
		}
	}
	else if (type == PARA_GOOMBA) {
		left = x;
		top = y;
		if (state == PARA_GOOMBA_STATE_WALKING) 
		{
			right = x + PARA_GOOMBA_BB_WIDTH;
			bottom = y + PARA_GOOMBA_BB_HEIGHT;
		}
		else {
			right = x + PARA_GOOMBA_BB_WIDTH;
			bottom = y + PARA_GOOMBA_BB_FLY_HEIGHT;
		}
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);

	DebugOut(L"\n Get kill %d", GetKillByKoopas);
	vy += MARIO_GRAVITY * dt;
	if (!GetKillByKoopas) {
		if (type == PARA_GOOMBA)
		{
			if (state == PARA_GOOMBA_STATE_WALKING)
			{
				if (GetTickCount64() - TimeWalking > WALKING_TIME_MAX)
				{
					StartReady();
				}
			}
			else if (state == PARA_GOOMA_STATE_READY_TO_FLY)
			{
				if (GetTickCount64() - TimeReadyToFly > READY_FLY_TIME_MAX)
				{
					StartFly();
				}
			}
			else if (state == PARA_GOOMBA_STATE_FLY)
			{
				if (GetTickCount64() - TimeFly > FLYING_TIME_MAX)
				{
					StartWalking();
				}
			}
			for (size_t i = 0; i < coObjects->size(); i++)
			{
				LPGAMEOBJECT obj = coObjects->at(i);
				float pLeft, pTop, pRight, pBottom;
				obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
				if (dynamic_cast<CPlatform*>(obj)) {
					if (CheckBB(pLeft, pTop, pRight, pBottom))
					{
						if (state == PARA_GOOMBA_STATE_WALKING)
						{
							y -= y + PARA_GOOMBA_BB_HEIGHT - pTop + PushBackPixel;
						}
						else {
							y -= y + PARA_GOOMBA_BB_FLY_HEIGHT - pTop + PushBackPixel;
						}
					}
				}
			}
		}
		else if (type == GOOMBA)
		{
			if (isSwitched)
			{
				if (GetTickCount64() - TimeToSwitch > 100)
				{
					this->isSwitched = false;
				}
			}
			if (state == GOOMBA_STATE_DIE)
			{
				vx = 0;
				vy = 0;
			}

			if (GetTickCount64() - TimeStartDisappear > 300 && GetTickCount64() - TimeStartDisappear < 500)
			{
				isDisapear = true;
				TimeStartDisappear = 0;
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

					if (plat->getType() == PLATFORM_TYPE_ONE)
					{
						if (state != PARA_GOOMBA_STATE_FLY) {
							x += min_tx * dx + nx * 0.4f;
							y += min_ty * dy + ny * 0.4f;
						}

						if (nx != 0)
						{
							vx = -vx;
							this->nx = -this->nx;
						}
						if (ny != 0) {
							if (state != PARA_GOOMBA_STATE_FLY) {
								vy = 0;
							}
							else {
								vy = -0.35f;
								x += dx;
								y += dy;
							}

						}

					}
				}
				else if (dynamic_cast<CFireBall*>(e->obj)) {
					CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
					if (nx != 0 || ny != 0) {
						SetState(GOOMBA_STATE_DIE);
					}
					fb->setIsAppear(false);
				}
				else if (dynamic_cast<CGoomba*>(e->obj)) {
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (this->type == GOOMBA) {
						if (goomba->GetType() == GOOMBA) {
							this->vx = -this->vx;
							goomba->vx = -goomba->vx;
						}
						else if (goomba->GetType() == PARA_GOOMBA) {
							x += dx;
							y += dy;
						}
					}
					else if (this->type == PARA_GOOMBA) {
						if (goomba->GetType() == GOOMBA) {
							x += dx;
							y += dy;
						}
					}
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	else {
		x += dx;
		y += dy;
	}
}

void CGoomba::Render()
{
	int ani = 0;
	if (type == GOOMBA) 
	{
		if (state != GOOMBA_STATE_DIE)
			ani = GOOMBA_ANI_WALKING;
		else
			ani = GOOMBA_ANI_DIE;
	}
	else if (type == PARA_GOOMBA) {
		if (state == PARA_GOOMBA_STATE_WALKING)
			ani = PARA_GOOMBA_ANI_WALKING;
		else if (state == PARA_GOOMA_STATE_READY_TO_FLY)
			ani = PARA_GOOMBA_ANI_READY_TO_FLY;
		else if (state == PARA_GOOMBA_STATE_FLY)
			ani = PARA_GOOMBA_ANI_FLY;
	}

	if (GetKillByKoopas) {
		if (type == GOOMBA)
			ani = GOOMBA_DEAD_BY_KOOPA_ANI;
		else
			ani = PARA_GOOMBA_DEAD_BY_KOOPA_ANI;
	}
	if(!isDisapear)
		animation_set->at(ani)->Render(x, y);
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case PARA_GOOMBA_STATE_FLY:
		vy = -0.5f;
		break;
	}
}
