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
#include "Brick.h"



CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	nx = 1;
	ay = MARIO_GRAVITY;
	setIsReadyToJump(true);
	setIsReadyToSit(true);
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
	
	// limit the speed of mario	 don't care about direction
	if (abs(vx) >= MARIO_WALKING_SPEED_MAX)
	{
		vx = nx * MARIO_WALKING_SPEED_MAX;
	}

	// If player want to slow down mario then acceleration and speed have opposite sign
	if (state == MARIO_STATE_IDLE) 
	{
		a = -nx * MARIO_ACCELERATION;
		ay = MARIO_GRAVITY;
		if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
			vx = 0;
	}

	//if (state == MARIO_STATE_SIT) {
	//	
	//	//DebugOut(L"\nVy = %f", vy);
	//}

	if (state == MARIO_STATE_SIT) {
		if (vy < 0)
			vy -= MARIO_ACCELERATION_JUMP * dt;
	}
	
	if (state == MARIO_STATE_JUMP) 
	{
		//DebugOut(L"\nVy = %f", vy);
		if (vy < 0)
			vy -= MARIO_ACCELERATION_JUMP * dt;
	}

	for (int i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CPlatform*>(obj)) {
			float pLeft, pTop, pRight, pBottom;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);

			if (CheckBB(pLeft, pTop, pRight, pBottom)) {
				y -= y + MARIO_BIG_BBOX_HEIGHT - pTop + 1.0f;
			}
		}
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

		if (ny < 0) {
			setIsReadyToJump(true);
			setIsReadyToSit(true);
		}
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
					x += dx;
					if (e->ny < 0) 
					{
						y += min_ty * dy + ny * 0.4f;
						vy = 0;
					}
					else {
						y += dy;
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
				else if (plat->getType() == PLATFORM_TYPE_ONE) {
					x += min_tx * dx + nx * 0.8f;
					y += min_ty * dy + ny * 0.1f;
					if (e->ny != 0)vy = 0;
					if (e->nx != 0)vx = 0;
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) {
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);

				x += min_tx * dx + nx * 0.8f;
				y += min_ty * dy + ny * 0.1f;
				if(e->ny<0)
				{
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
				x += dx;
				y += dy;
				if (e->nx != 0 || e->ny != 0) {
					coin->SetState(COIN_STATE_DISAPPEAR);
				}
				
			}
			else if (dynamic_cast<CBrick*>(e->obj)) {
				x += min_tx * dx + nx * 0.8f;
				y += min_ty * dy + ny * 0.1f;
				if (e->ny != 0)vy = 0;
				if (e->nx != 0)vx = 0;
			}
		}	
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::SameRenderLogicsForAllLevel(int &ani, int ani_jump_down_right, int ani_jump_down_left,
									int ani_idle_right, int ani_idle_left,
									int ani_stop_right, int ani_stop_left, int ani_walking_right, int ani_walking_left) 
{
		//when mario is falling down or 
		//on the ground and not doing anything
		if (vx == 0) {
			if (IsReadyToJump() && vy >= 0) //if he's on the ground
			{
				if (nx > 0) ani = ani_idle_right;
				else ani = ani_idle_left;
			}
			else //when he falling down
			{
				if (nx > 0) ani = ani_jump_down_right;
				else ani = ani_jump_down_left;
			}
		}

		//when mario moving on the ground
		else {
			if (vx > 0 && nx < 0) {
				ani = ani_stop_right;
			}
			else if (vx < 0 && nx >0) {
				ani = ani_stop_left;
			}
			else if (vx > 0 && nx > 0) {
				ani = ani_walking_right;
			}
			else if (vx < 0 && nx < 0) {
				ani = ani_walking_left;
			}
		}
	
}

void CMario::RenderLogicForSittingState(int& ani, int ani_sit_right, int ani_sit_left)
{
	if (nx > 0)
		ani = ani_sit_right;
	else
		ani = ani_sit_left;
}
void CMario::RenderLogicForJumpingState(int& ani, int ani_jump_up_right, int ani_jump_up_left,int ani_jump_down_right,int ani_jump_down_left)
{
		if (nx > 0 && vy < 0)
			ani = ani_jump_up_right;
		else if (nx < 0 && vy < 0)
			ani = ani_jump_up_left;
		else if (nx > 0 && vy > 0)
			ani = ani_jump_down_right;
		else
			ani = ani_jump_down_left;
	
}
void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;

	//Render for mario big
	if (level == MARIO_LEVEL_BIG) 
	{
		if (state == MARIO_STATE_SIT) 
		{
			RenderLogicForSittingState(ani, 
				MARIO_ANI_BIG_SIT_RIGHT, 
				MARIO_ANI_BIG_SIT_LEFT);
		}
		else if (state == MARIO_STATE_JUMP) 
		{
			RenderLogicForJumpingState(ani, 
				MARIO_ANI_BIG_JUMP_RIGHT, 
				MARIO_ANI_BIG_JUMP_LEFT, 
				MARIO_ANI_BIG_JUMP_DOWN_RIGHT, 
				MARIO_ANI_BIG_JUMP_DOWN_LEFT);
		}
		else 
		{
			SameRenderLogicsForAllLevel(ani,
				MARIO_ANI_BIG_JUMP_DOWN_RIGHT, MARIO_ANI_BIG_JUMP_DOWN_LEFT,
				MARIO_ANI_BIG_IDLE_RIGHT, MARIO_ANI_BIG_IDLE_LEFT,
				MARIO_ANI_BIG_STOP_RIGHT, MARIO_ANI_BIG_STOP_LEFT,
				MARIO_ANI_BIG_WALKING_RIGHT, MARIO_ANI_BIG_WALKING_LEFT);
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
		{
			if (state == MARIO_STATE_JUMP) {
				RenderLogicForJumpingState(ani,
					MARIO_ANI_SMALL_JUMP_RIGHT,
					MARIO_ANI_SMALL_JUMP_LEFT,
					MARIO_ANI_SMALL_JUMP_DOWN_RIGHT,
					MARIO_ANI_SMALL_JUMP_DOWN_LEFT);
			}
			else 
			// small mario is moving
			{
				SameRenderLogicsForAllLevel(ani,
					MARIO_ANI_SMALL_JUMP_DOWN_RIGHT, MARIO_ANI_SMALL_JUMP_DOWN_LEFT,
					MARIO_ANI_SMALL_IDLE_RIGHT, MARIO_ANI_SMALL_IDLE_LEFT,
					MARIO_ANI_SMALL_STOP_RIGHT, MARIO_ANI_SMALL_STOP_LEFT,
					MARIO_ANI_SMALL_WALKING_RIGHT, MARIO_ANI_SMALL_WALKING_LEFT);
			}
			
		}
	else if (level == MARIO_LEVEL_FIRE)
	{
		
		if (state == MARIO_STATE_SIT)
		{
			RenderLogicForSittingState(ani,
				MARIO_ANI_FIRE_SIT_RIGHT,
				MARIO_ANI_FIRE_SIT_LEFT);
		}
		else if (state == MARIO_STATE_JUMP)
		{
			RenderLogicForJumpingState(ani,
				MARIO_ANI_FIRE_JUMP_RIGHT,
				MARIO_ANI_FIRE_JUMP_LEFT,
				MARIO_ANI_FIRE_JUMP_DOWN_RIGHT,
				MARIO_ANI_FIRE_JUMP_DOWN_LEFT);
		}
		else
		{
			SameRenderLogicsForAllLevel(ani,
				MARIO_ANI_FIRE_JUMP_DOWN_RIGHT, MARIO_ANI_FIRE_JUMP_DOWN_LEFT,
				MARIO_ANI_FIRE_IDLE_RIGHT, MARIO_ANI_FIRE_IDLE_LEFT,
				MARIO_ANI_FIRE_STOP_RIGHT, MARIO_ANI_FIRE_STOP_LEFT,
				MARIO_ANI_FIRE_WALKING_RIGHT, MARIO_ANI_FIRE_WALKING_LEFT);
		}
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
		if (state == MARIO_STATE_SIT)
		{
			RenderLogicForSittingState(ani,
				MARIO_ANI_TAIL_SIT_RIGHT,
				MARIO_ANI_TAIL_SIT_LEFT);
		}
		else if (state == MARIO_STATE_JUMP)
		{
			RenderLogicForJumpingState(ani,
				MARIO_ANI_TAIL_JUMP_RIGHT,
				MARIO_ANI_TAIL_JUMP_LEFT,
				MARIO_ANI_TAIL_JUMP_DOWN_RIGHT,
				MARIO_ANI_TAIL_JUMP_DOWN_LEFT);
		}
		else
		{
			SameRenderLogicsForAllLevel(ani,
				MARIO_ANI_TAIL_JUMP_DOWN_RIGHT, MARIO_ANI_TAIL_JUMP_DOWN_LEFT,
				MARIO_ANI_TAIL_IDLE_RIGHT, MARIO_ANI_TAIL_IDLE_LEFT,
				MARIO_ANI_TAIL_STOP_RIGHT, MARIO_ANI_TAIL_STOP_LEFT,
				MARIO_ANI_TAIL_WALKING_RIGHT, MARIO_ANI_TAIL_WALKING_LEFT);
		}
	}   
	else if (level == MARIO_LEVEL_HAMMER)
	{
		if (state == MARIO_STATE_SIT)
		{
			RenderLogicForSittingState(ani,
				MARIO_ANI_HAMMER_SIT_RIGHT,
				MARIO_ANI_HAMMER_SIT_LEFT);
		}
		else if (state == MARIO_STATE_JUMP)
		{
			RenderLogicForJumpingState(ani,
				MARIO_ANI_HAMMER_JUMP_RIGHT,
				MARIO_ANI_HAMMER_JUMP_LEFT,
				MARIO_ANI_HAMMER_JUMP_DOWN_RIGHT,
				MARIO_ANI_HAMMER_JUMP_DOWN_LEFT);
		}
		else
		{
			SameRenderLogicsForAllLevel(ani,
				MARIO_ANI_HAMMER_JUMP_DOWN_RIGHT, MARIO_ANI_HAMMER_JUMP_DOWN_LEFT,
				MARIO_ANI_HAMMER_IDLE_RIGHT, MARIO_ANI_HAMMER_IDLE_LEFT,
				MARIO_ANI_HAMMER_STOP_RIGHT, MARIO_ANI_HAMMER_STOP_LEFT,
				MARIO_ANI_HAMMER_WALKING_RIGHT, MARIO_ANI_HAMMER_WALKING_LEFT);
		}
	}
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
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
		/*if (a * nx > 0)
			a = -nx * MARIO_ACCELERATION;*/
		//setIsReadyToJump(true);
		break;
	case MARIO_STATE_SIT:
		if (abs(vx) <= MARIO_WALKING_SPEED_MIN) {
			vx = 0;
		}
		ay = MARIO_GRAVITY;
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
		if (state == MARIO_STATE_SIT)
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT+4;
		}
		else {
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}

bool CMario::CheckBB(float friend_left, float friend_top, float friend_right, float friend_bottom) {
	float this_left, this_top, this_right, this_bottom;
	GetBoundingBox(this_left, this_top, this_right, this_bottom);
	bool on1 = friend_left < this_right;
	bool on2 = friend_top < this_bottom;
	bool down1 = friend_right > this_left;
	bool down2 = friend_bottom > this_top;
	
	return on1 && on2 && down1 && down2 ;
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
