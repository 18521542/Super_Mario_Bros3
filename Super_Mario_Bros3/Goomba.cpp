#include "Goomba.h"
#include "Mario.h"
#include "Platform.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
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

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);

	vy += MARIO_GRAVITY * dt;
	vx = 0.03f;


	for (size_t i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		float pLeft, pTop, pRight, pBottom;
		obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
		if (dynamic_cast<CPlatform*>(obj)) {
			if (CheckBB(pLeft, pTop, pRight, pBottom))
			{
				//Push back a little bit when bounding box has collision
				y = GOOMBA_BBOX_HEIGHT - pTop + PushBackPixel;
				if (nx > 0) {
					x = GOOMBA_BBOX_WIDTH - pLeft - PushBackPixel;
				}
				else
					x = GOOMBA_BBOX_WIDTH - pRight + PushBackPixel;
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
				
				if (plat->getType() == PLATFORM_TYPE_ONE)
				{
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;

					if (nx != 0 )
					{
						vx = -vx;
						this->nx = -this->nx;
					}
					if (ny != 0)
						vy = 0;
				}
			}
			else if (dynamic_cast<CFireBall*>(e->obj)) {
				CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
				if (nx != 0 || ny != 0) {
					SetState(GOOMBA_STATE_DIE);
				}

				fb->setIsAppear(false);

			}
			
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if(state != GOOMBA_STATE_DIE)
		animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
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
	}
}
