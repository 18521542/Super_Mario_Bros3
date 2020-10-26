#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "GameObject.h"
#include "Platform.h"
#include "Koopas.h"
#include "coin.h"



CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	nx = 1;
	ay = MARIO_GRAVITY;
	setIsReadyToJump(true);
	a = MARIO_ACCELERATION;
	//ay = MARIO_GRAVITY;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	// Calculate dx, dy by speed and acceleration
	CGameObject::Update(dt);

	//a = nx * MARIO_ACCELERATION;
	vx += a * dt;
	// Simple fall down	
	vy += ay * dt;
	DebugOut(L"\nVy = %f", vy);
	// limit the speed of mario
	// don't care about direction
	if (abs(vx) >= MARIO_WALKING_SPEED_MAX)
	{
		vx = nx * MARIO_WALKING_SPEED_MAX;
	}
	if (state == MARIO_STATE_IDLE)// If player want to slow down mario then acceleration and speed have opposite sign 
	{
		a = -nx * MARIO_ACCELERATION;
		ay = MARIO_GRAVITY;
		if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
			vx = 0;
	}
	if (state == MARIO_STATE_JUMP) 
	{
		DebugOut(L"\nVy = %f", vy);
		if (vy < 0)
			vy -= MARIO_ACCELERATION_JUMP * dt;
		
	}

	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
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

		//
		// Collision logic with  special-objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<CPlatform*>(e->obj)) {
				CPlatform* plat = dynamic_cast<CPlatform*>(e->obj);
				if (plat->getType() == PLATFORM_TYPE_TWO) 
				{
					if (e->ny > 0) {
						x += dx;
						y += dy;
						ny = 0;
						nx = 0;
					}
				}
				else if (plat->getType() == PLATFORM_TYPE_THREE) {
					//ignore this platform
					if (e->ny != 0||e->nx != 0) {
						x += dx;
						y += dy;
						ny = 0;
						nx = 0;
					}
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				float currentX, currentY;
				if(e->ny<0)
				{
					/*koopas->GetPosition(currentX, currentY);
					koopas->SetPosition(currentX, currentY - 20);*/
					if (koopas->GetState() == KOOPAS_STATE_WALKING) {
						koopas->SetState(KOOPAS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
						
					else if (koopas->GetState() == KOOPAS_STATE_DIE)
					{
						koopas->SetState(KOOPAS_STATE_DIE_MOVING);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
						
					
				}
			}
			else if(dynamic_cast<CCoin*>(e->obj)){
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				if (e->nx != 0 || e->ny != 0) {
					coin->SetState(COIN_STATE_DISAPPEAR);
					nx = 0;
					ny = 0;
				}
				
			}

		}


		// block every none-special object and set IsReadyToJump = true if mario below them!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if(ny<0)
			setIsReadyToJump(true);
		//DebugOut(L"\nIsReadyToJump = %d", isReadyToJump);
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;

		if (level == MARIO_LEVEL_BIG)
		{
			if (state == MARIO_STATE_JUMP ) //when mario jump
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else
					ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
			else if (vx == 0)	//when mario is not doing anything
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else   //when mario moving 
			{
				if (vx > 0 && nx < 0) {
					ani = MARIO_ANI_BIG_STOP_RIGHT;
				}
				else if (vx < 0 && nx >0) {
					ani = MARIO_ANI_BIG_STOP_LEFT;
				}
				else if (vx > 0 && nx > 0) {
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
				}
				else if (vx < 0 && nx < 0) {
					ani = MARIO_ANI_BIG_WALKING_LEFT;
				}
			}
			
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else 
			// small mario is moving
			{
				if (vx > 0 && nx < 0) {
					ani = MARIO_ANI_SMALL_STOP_RIGHT;
				}
				else if (vx < 0 && nx >0) {
					ani = MARIO_ANI_SMALL_STOP_LEFT;
				}
				else if (vx > 0 && nx > 0) {
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				else if (vx < 0 && nx < 0) {
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
			}
			
		}
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_FROG)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_FROG_IDLE_RIGHT;
				else ani = MARIO_ANI_FROG_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_FROG_WALKING_RIGHT;
			else ani = MARIO_ANI_FROG_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
				else ani = MARIO_ANI_TAIL_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_TAIL_WALKING_RIGHT;
			else ani = MARIO_ANI_TAIL_WALKING_LEFT;
		}
		else if (level == MARIO_LEVEL_HAMMER)
		{
			if (vx == 0)
			{
				if (nx > 0) ani = MARIO_ANI_HAMMER_IDLE_RIGHT;
				else ani = MARIO_ANI_HAMMER_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = MARIO_ANI_HAMMER_WALKING_RIGHT;
			else ani = MARIO_ANI_HAMMER_WALKING_LEFT;
		}
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		a = MARIO_ACCELERATION;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		a = -MARIO_ACCELERATION;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		a = 0;
		if(vy==0)
		vy = -MARIO_JUMP_SPEED_MAX;
		break;
	case MARIO_STATE_IDLE:
		if (a * nx > 0)
			a = -nx * MARIO_ACCELERATION;
		//setIsReadyToJump(true);
		break;
	case MARIO_STATE_FALING_DOWN:
		ay = MARIO_GRAVITY;
		a = MARIO_ACCELERATION;
		//setIsReadyToJump(true);
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (level != MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	nx = 1;
	a = MARIO_ACCELERATION;
	//setIsReadyToJump(true);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

