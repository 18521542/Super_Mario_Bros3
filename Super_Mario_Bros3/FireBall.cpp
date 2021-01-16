#pragma once
#include "FireBall.h"
#include "Utils.h"
#include "Mario.h"
#include "Platform.h"
#include "Koopas.h"
#include "PlayScene.h"
CFireBall::CFireBall()
{
	isAppear = true;
	//vx = 0;
	//vy = 0;
}

CFireBall::CFireBall(int typ) {
	this->type = typ;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(7));
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
	RenderBoundingBox();
}

void CFireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (type == FB_OF_VENUS) {
		l = x;
		t = y;
		r = x + FB_BBOX_WIDTH;
		b = y + FB_BBOX_HEIGHT;
		//DebugOut(L"\nAAAA");
	}
	else {
		if (!isAppear) {
			l = 0;
			t = 0;
			r = 0;
			b = 0;
		}
		else {
			l = x;
			t = y;
			r = x + FB_BBOX_WIDTH;
			b = y + FB_BBOX_HEIGHT;
		}
	}
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (type != FB_OF_VENUS) {
		
		//do nothing;

		if (GetTickCount64() - startFly > FB_APPEAR_TIME) {
			isAppear = false;
			startFly = 0;
		}

		vy += MARIO_GRAVITY * dt;


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

			if (nx != 0) {
				isAppear = false;
			}


			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<CFireBall*>(e->obj))
				{
					CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
					x += dx;
					y += dy;
				}
				else if (dynamic_cast<CPlatform*>(e->obj)) {
					CPlatform* plat = dynamic_cast<CPlatform*>(e->obj);

					//DebugOut(L"\n FB is on the ground");
					if (nx != 0) {
						isAppear = false;
					}
					if (ny != 0) {
						this->vy = -UP_SPEED;
					}
				}
			}
		}
	}
	else {
		x += dx;
		y += dy;
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float le, to, ri, bo;
		mario->GetBoundingBox(le, to, ri, bo);
		if (CheckBB(le, to, ri, bo) && !isCollide) 
		{
			//mario->SetState(MARIO_STATE_DIE);
			mario->StartDecreaseLevel();
			isCollide = true;
		}
	}
}