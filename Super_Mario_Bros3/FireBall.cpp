#pragma once
#include "FireBall.h"
#include "Utils.h"
#include "Mario.h"

CFireBall::CFireBall()
{
	isAppear = true;
	vx = 0;
	vy = 0;
}

CFireBall::CFireBall(float xO, float yO)
{
	SetPosition(xO, yO);
	isAppear = true;
}
void CFireBall::Render()
{
	if (isAppear)
		animation_set->at(FB_ANI)->Render(x, y);
	//RenderBoundingBox();
}

void CFireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FB_BBOX_WIDTH;
	b = y + FB_BBOX_HEIGHT;
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	//do nothing;

	x += dx;
	y += dy;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if(isAppear)
		CalcPotentialCollisions(coObjects, coEvents);

	coEvents.clear();

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


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
				CMario* mario = dynamic_cast<CMario*>(e->obj);
				vx = 0;
				vy = 0;

			}
			else if (dynamic_cast<CFireBall*>(e->obj))
			{
				CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
				x += dx;
				y += dy;
			}
		}
	}
}