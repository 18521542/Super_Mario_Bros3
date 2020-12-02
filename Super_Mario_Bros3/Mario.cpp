#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Portal.h"
#include "Platform.h"





CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	nx = 1;
	ay = MARIO_GRAVITY;
	setIsReadyToJump(true);
	setIsReadyToSit(true);
	//SetIsStartUsingTail(false);
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
	
	// limit the speed of mario when he walk - don't care about direction
	if (abs(vx) >= MARIO_WALKING_SPEED_MAX && !isRunning)
	{
		vx = nx * MARIO_WALKING_SPEED_MAX;
	}

	//when mario run
	if (abs(vx) >= MARIO_RUN_SPEED_MAX) 
	{
		vx = nx * MARIO_RUN_SPEED_MAX;
		isReadyToJumpFly = true;
		if (level == MARIO_LEVEL_TAIL) 
		{
			isReadyToFly = true;
		}
	}

	// If player want to slow down mario then acceleration and speed have opposite sign
	if (state == MARIO_STATE_IDLE) 
	{
		a = -nx * MARIO_ACCELERATION;
		ay = MARIO_GRAVITY;
		if (abs(vx) <= MARIO_WALKING_SPEED_MIN)
			vx = 0;
	}

	else if (state == MARIO_STATE_SIT) {
		if (vy < 0)
			vy -= MARIO_JUMPING_ACCELERATION * dt;
	}
	
	else if (state == MARIO_STATE_JUMP) 
	{
		if (vy < 0)
			vy -= MARIO_JUMPING_ACCELERATION * dt;
		
	}
	//handle overlap logic
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		float pLeft, pTop, pRight, pBottom;
		obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
		if (dynamic_cast<CKoopas*>(obj)) 
		{
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			CKoopas* kp = dynamic_cast<CKoopas*>(obj);
			if (CheckBB(pLeft, pTop, pRight, pBottom))
			{
				if (isUsingTail)
				{
					obj->SetDirection(nx);
					obj->SetState(KOOPAS_STATE_DIE_UP);
				}
				if (isReadyToHold)
				{
					if (kp->GetState()==KOOPAS_STATE_DIE||kp->GetState()==KOOPAS_STATE_DIE_UP)
					{
						if (level != MARIO_LEVEL_SMALL)
						{
							obj->SetPosition(x + nx * (MARIO_BIG_BBOX_WIDTH - SAFETY_DISTANCE_TO_HOLD), y + 10.0f);
						}
						else
						{
							obj->SetPosition(x + nx * (MARIO_SMALL_BBOX_WIDTH-SAFETY_DISTANCE_TO_HOLD), y - 1.0f);
						}
					}
				}
			}
		}

		else if (dynamic_cast<CFireBall*>(obj)) {
			CFireBall* fb = dynamic_cast<CFireBall*>(obj);
			
			if (isShootingFireBall && !fb->IsAppear() && isForFireBallAppear)
			{
				obj->SetPosition(x + nx * (MARIO_BIG_BBOX_WIDTH + 1.0f), y);
				obj->SetSpeed(nx*FB_SPEED_X, FB_SPEED_Y);
				fb->StartAppear();
				isForFireBallAppear = false;
			}
		}
		
		else if (dynamic_cast<CPlatform*>(obj)) 
		{
			CPlatform* pf = dynamic_cast<CPlatform*>(obj);
			if (CheckBB(pLeft, pTop, pRight, pBottom) && pf->getType()!=PLATFORM_TYPE_TWO)
			{
				y -= y + MARIO_BIG_BBOX_HEIGHT - pTop + PushBackPixel;
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
	
	//reset IsStartUsingTail = false if mario finish using his tail
	if (GetTickCount() - using_tail_start > MARIO_USING_TAIL_TIME)
	{
		using_tail_start = 0;
		isUsingTail = false;
	}
	
	//reset shooting to  if mario finish shotting
	if (GetTickCount() - shooting_start > MARIO_SHOOTING_TIME)
	{
		isShootingFireBall = false;
		shooting_start = 0;
		isForFireBallAppear = false;
		//isStartShooting = false;
	}

	//if flying time is over then...
	if (GetTickCount() - StartFly > MARIO_FLYING_TIME) {
		isFlying = false;
		StartFly = 0;
		//isReadyToFly = false;
	}

	//Kicking time is over then...
	if (GetTickCount() - StartKick > MARIO_KICKING_TIME) {
		StartKick = 0;
		isKicking = false;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}

	// if collision
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//Reset status when mario is on the ground
		if (ny < 0 ) 
		{
			setIsReadyToJump(true);
			//state = MARIO_STATE_IDLE;
			setIsReadyToSit(true);
			setIsJumpFlying(false);
			setIsReadyToJumpFlying(false);
			//if flying time is not over then
			if (GetTickCount() - StartFly <= MARIO_FLYING_TIME) {
				isFlying = false;
			}
			setIsOnAir(false);
		}
		if (nx != 0 || ny!=0) 
		{
			setIsReadyToJumpFlying(false);
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
					if (e->ny == 0) {
						x += dx;
						y += dy;
						isReadyToSit = true;
						isReadyToJump = true;
					}
					else if (e->ny < 0) {
						x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;
						vy = 0;
						isReadyToSit = true;
						isReadyToJump = true;

						
					}
					else if(e->ny>0)
					{
						x += min_tx * dx + nx * 0.4f;
						y += dy;
						ny = 0;
						isReadyToSit = true;
						isReadyToJump = true;

						
					}
					
					
					
				}
				else if (plat->getType() == PLATFORM_TYPE_THREE) {
					//ignore this platform
					if (e->ny != 0||e->nx != 0) {
						x += dx;
						y += dy;
					}
				}
				else if (plat->getType() == PLATFORM_TYPE_ONE) {
					x += min_tx * dx + nx * 0.8f;
					y += min_ty * dy + ny * 0.1f;
					if (e->ny != 0)vy = 0;
					if (e->nx != 0)vx = 0;
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj)) 
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				float kVx, kVy;
				koopas->GetSpeed(kVx, kVy);

				float kpL, kpT, kpR, kpB;
				koopas->GetBoundingBox(kpL, kpT, kpR, kpB);
				
				if(e->ny<0)
				{
					if (koopas->GetState() == KOOPAS_STATE_WALKING) {
						koopas->SetState(KOOPAS_STATE_DIE);
						
					}
					else if (koopas->GetState() == KOOPAS_STATE_DIE)
					{
						koopas->SetState(KOOPAS_STATE_DIE_MOVING);
					}
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
				if (!isReadyToHold) 
				{
					if (e->nx != 0)
					{
						//if koopas is moving then mario die
						if (abs(kVx) > 0)
						{
							if (untouchable == 0)
							{
								if (koopas->GetState() != GOOMBA_STATE_DIE)
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
						else 
						{
							koopas->SetDirection(this->nx);
							koopas->SetState(KOOPAS_STATE_DIE_MOVING);
							StartKicking();
						}
					}
				}
				else {
					x += dx;
					y += dy;
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
			else if (dynamic_cast<CFireBall*>(e->obj)){
				CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
				x += dx;
				y += dy;
				/*fb->setIsAppear(true);
				fb->SetPosition(this->x, this->y);
				fb->SetSpeed(0, 0);*/
			}
		}	
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//DebugOut(L"\n mario is using tail = %d", IsUsingTail());
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

void CMario::RenderLogicForRunningState(int& ani, int ani_run_right, int ani_run_left, int ani_walk_fast_right, int ani_walk_fast_left) 
{
	if (nx > 0 && vx == MARIO_RUN_SPEED_MAX) {
		ani = ani_run_right;
	}
	else if (nx < 0 && vx == -MARIO_RUN_SPEED_MAX) {
		ani = ani_run_left;
	}
	else if (nx > 0 && vx != MARIO_RUN_SPEED_MAX) {
		ani = ani_walk_fast_right;
	}
	else if (nx < 0 && vx != -MARIO_RUN_SPEED_MAX) {
		ani = ani_walk_fast_left;
	}
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
			if (isJumpFlying)
			{
				RenderLogicForSittingState(ani, MARIO_ANI_BIG_JUMP_FLY_RIGHT, MARIO_ANI_BIG_JUMP_FLY_LEFT);
			}
			else
			RenderLogicForJumpingState(ani, 
				MARIO_ANI_BIG_JUMP_RIGHT, 
				MARIO_ANI_BIG_JUMP_LEFT, 
				MARIO_ANI_BIG_JUMP_DOWN_RIGHT, 
				MARIO_ANI_BIG_JUMP_DOWN_LEFT);
		}
		else if (isRunning)
		{
			RenderLogicForRunningState(ani, MARIO_ANI_BIG_RUN_RIGHT, MARIO_ANI_BIG_RUN_LEFT,
				MARIO_ANI_BIG_WALK_FAST_RIGHT, MARIO_ANI_BIG_WALK_FAST_LEFT);
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
			else {
				if (isRunning) {
					RenderLogicForRunningState(ani, MARIO_ANI_SMALL_RUN_RIGHT, MARIO_ANI_SMALL_RUN_LEFT, 
						MARIO_ANI_SMALL_WALK_FAST_RIGHT, MARIO_ANI_SMALL_WALK_FAST_LEFT);
				}
				else
				{
					SameRenderLogicsForAllLevel(ani,
						MARIO_ANI_SMALL_JUMP_DOWN_RIGHT, MARIO_ANI_SMALL_JUMP_DOWN_LEFT,
						MARIO_ANI_SMALL_IDLE_RIGHT, MARIO_ANI_SMALL_IDLE_LEFT,
						MARIO_ANI_SMALL_STOP_RIGHT, MARIO_ANI_SMALL_STOP_LEFT,
						MARIO_ANI_SMALL_WALKING_RIGHT, MARIO_ANI_SMALL_WALKING_LEFT);
				}
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
			if (isShootingFireBall) 
			{
				if (nx > 0) {
					ani = MARIO_ANI_FIRE_SHOOT_RIGHT;

				}
					
				else {
					ani = MARIO_ANI_FIRE_SHOOT_LEFT;
				}
					
			}
			else {
				if (isRunning) {
					RenderLogicForRunningState(ani, MARIO_ANI_FIRE_RUN_RIGHT, MARIO_ANI_FIRE_RUN_LEFT,
						MARIO_ANI_FIRE_WALK_FAST_RIGHT, MARIO_ANI_FIRE_WALK_FAST_LEFT);
				}
				else
				SameRenderLogicsForAllLevel(ani,
					MARIO_ANI_FIRE_JUMP_DOWN_RIGHT, MARIO_ANI_FIRE_JUMP_DOWN_LEFT,
					MARIO_ANI_FIRE_IDLE_RIGHT, MARIO_ANI_FIRE_IDLE_LEFT,
					MARIO_ANI_FIRE_STOP_RIGHT, MARIO_ANI_FIRE_STOP_LEFT,
					MARIO_ANI_FIRE_WALKING_RIGHT, MARIO_ANI_FIRE_WALKING_LEFT);
			}
			
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
			RenderLogicForSittingState(ani, MARIO_ANI_TAIL_SIT_RIGHT, MARIO_ANI_TAIL_SIT_LEFT);
		}
		else if (state == MARIO_STATE_JUMP)
		{
			if (isJumpFlying) 
			{
				RenderLogicForSittingState(ani, MARIO_ANI_TAIL_JUMP_FLY_RIGHT, MARIO_ANI_TAIL_JUMP_FLY_LEFT);
			}
			else
			RenderLogicForJumpingState(ani,
				MARIO_ANI_TAIL_JUMP_RIGHT,
				MARIO_ANI_TAIL_JUMP_LEFT,
				MARIO_ANI_TAIL_JUMP_DOWN_RIGHT,
				MARIO_ANI_TAIL_JUMP_DOWN_LEFT);
		}
		else if (isFlying) {
			if (nx > 0) {
				ani = MARIO_ANI_TAIL_JUMP_FLY_RIGHT;
			}
			else
				ani = MARIO_ANI_TAIL_JUMP_FLY_LEFT;
		}
		else if (isKicking) {
			if (nx > 0)
				ani = MARIO_ANI_TAIL_KICK_RIGHT;
			else
				ani = MARIO_ANI_TAIL_KICK_LEFT;
		}
		else if (IsUsingTail()) {
			if (nx > 0)
				ani = MARIO_ANI_TAIL_USETAIL_RIGHT;
			else
				ani = MARIO_ANI_TAIL_USETAIL_LEFT;
		}
		else if (isRunning) {
			RenderLogicForRunningState(ani, MARIO_ANI_TAIL_RUN_RIGHT, MARIO_ANI_TAIL_RUN_LEFT,
				MARIO_ANI_TAIL_WALK_FAST_RIGHT, MARIO_ANI_TAIL_WALK_FAST_LEFT);
		}
		else if ((isOnAir && isFalling))
		{
			if (nx > 0) {
				ani = MARIO_ANI_TAIL_FALLING_RIGHT;
			}
			else
				ani = MARIO_ANI_TAIL_FALLING_LEFT;
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
		if (vx == 0) {
			a = 0;
		}
		else
			a = nx * MARIO_ACCELERATION;
		if(vy==0)
			vy =  -MARIO_JUMP_SPEED_MAX;
		if (level == MARIO_LEVEL_TAIL) 
		{
			isOnAir = true;
		}
		break;
	case MARIO_STATE_IDLE:
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
	case MARIO_STATE_FLY:
		//isReadyToFly = false;
		a = 0;
		vy = -MARIO_FLY_SPEED;
		isOnAir = true;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_TAIL)
	{
		left = x;
		top = y;
		if (!isUsingTail) 
		{
			if (state != MARIO_STATE_SIT)
			{
				right = x + MARIO_TAIL_BBOX_WIDTH;
				bottom = y + MARIO_BIG_BBOX_HEIGHT;
			}
			else 
			{
				right = x + MARIO_BIG_BBOX_WIDTH;
				bottom = y + MARIO_SMALL_BBOX_HEIGHT + 4;
			}
		}
		else 
		{
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
			top = y;
			if (nx > 0) {
				left = x;
				right = x + MARIO_TAIL_BBOX_WIDTH + TAIL_BBOX_WIDTH;
			}
			else 
			{
				left = (x - TAIL_BBOX_WIDTH);
				right = x + MARIO_TAIL_BBOX_WIDTH;
			}
			
		}
	}
	else 
	{
		left = x;
		top = y;
		if (level == MARIO_LEVEL_SMALL)
		{
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}
		else
	{
		if (state == MARIO_STATE_SIT)
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT + 4;
		}
		else {
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	//SetIsStartUsingTail(false);
	nx = 1;
	a = MARIO_ACCELERATION;
	//setIsReadyToJump(true);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

