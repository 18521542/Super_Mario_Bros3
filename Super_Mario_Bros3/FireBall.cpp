#pragma once
#include "FireBall.h"

CFireBall::CFireBall()
{
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
	vx = FB_SPEED_X;
	vy = FB_SPEED_Y;

	x += dx;
	y += dy;
}