#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Portal.h"
#include "Platform.h"
#include "PlayScene.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	nx = 1;
	ay = MARIO_GRAVITY;
	isReadyToJump = true;
	setIsReadyToSit(true);
	a = MARIO_ACCELERATION;
	isReadyToFly = false;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::UpdateStateUsingTimeOut() 
{
	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	
	//reset IsStartUsingTail = false if mario finish using his tail
	if (GetTickCount64() - using_tail_start > MARIO_USING_TAIL_TIME)
	{
		using_tail_start = 0;
		isUsingTail = false;
	}

	if (GetTickCount64() - tail_appear > MARIO_FOR_TAIL_APPEAR_TIME) {
		tail_appear = 0;
		isForTailAppear = false;
	}

	//reset shooting to  if mario finish shotting
	if (GetTickCount64() - shooting_start > MARIO_SHOOTING_TIME)
	{
		isShootingFireBall = false;
		shooting_start = 0;
		isForFireBallAppear = false;
		//isStartShooting = false;
	}

	//if flying time is over then...
	if (GetTickCount64() - StartFly > MARIO_FLYING_TIME) {
		isFlying = false;
		StartFly = 0;
	}

	if (GetTickCount64() - StartReadyToFly > 500) {
		isReadyToFly = false;
		StartReadyToFly = 0;
	}

	//Kicking time is over then...
	if (GetTickCount64() - StartKick > MARIO_KICKING_TIME) {
		StartKick = 0;
		isKicking = false;
	}

	if (GetTickCount64() - EnterTime > 3000 && GetTickCount64() - EnterTime<5000) 
	{
		isEntering = false;
		EnterTime = 0;
		if (!isInSecretRoom) 
		{
			isInSecretRoom = true;
			x = 2300;
			y = 611;
		}
		else {
			isInSecretRoom = false;
			x = 2310;
			y = 400;
		}
	}
}

void CMario::UpdateForEachState(DWORD dt) {
	
	// limit the speed of mario when he walk - don't care about direction
	if (abs(vx) >= MARIO_WALKING_SPEED_MAX && !isRunning)
	{
		vx = nx * MARIO_WALKING_SPEED_MAX;
	}

	if (abs(vx) <= MARIO_WALKING_SPEED_MIN) 
	{
		if(!(state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_WALKING_LEFT))
			vx = 0;
	}

	//when mario run
	if (abs(vx) >= MARIO_RUN_SPEED_MAX)
	{
		vx = nx * MARIO_RUN_SPEED_MAX;
		isReadyToJumpFly = true;
		if (level == MARIO_LEVEL_TAIL)
		{
			GetReadyToFly();
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

	else if (state == MARIO_STATE_SIT) 
	{
		if (vy < 0)
			vy -= MARIO_JUMPING_ACCELERATION * dt;
	}

	else if (state == MARIO_STATE_JUMP || state == MARIO_STATE_JUMP_FLY)
	{
		if (vy < 0)
			vy -= MARIO_JUMPING_ACCELERATION * dt;
	}
}

void CMario::HandleOverlapColision(vector<LPGAMEOBJECT>* coObjects) 
{
	for (size_t i = 0; i < coObjects->size(); i++)
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
				//DebugOut(L"\nIs hold to hold: %d", isHolding);
				if (isReadyToHold)
				{
					if (kp->GetState() == KOOPAS_STATE_DIE || kp->GetState() == KOOPAS_STATE_DIE_UP)
					{
						isHolding = true;
						//set position for koopas.
						if (level != MARIO_LEVEL_SMALL)
						{
							obj->SetPosition(x + nx * (MARIO_BIG_BBOX_WIDTH - SAFETY_DISTANCE_TO_HOLD), y + SAFETY_HEIGHT_FOR_BIG_TO_HOLD);
						}
						else
						{
							obj->SetPosition(x + nx * (MARIO_SMALL_BBOX_WIDTH - SAFETY_DISTANCE_TO_HOLD), y - SAFETY_HEIGHT_FOR_SMALL_TO_HOLD);
						}
					}
				}
				else {
					if (isHolding) {
						isHolding = false;
						StartKicking();
						kp->SetDirection(this->nx);
						kp->SetState(KOOPAS_STATE_DIE_MOVING);
					}
				}
			}
		}
		else if (dynamic_cast<CFireBall*>(obj)) {
			CFireBall* fb = dynamic_cast<CFireBall*>(obj);

			if (isShootingFireBall && !fb->IsAppear() && isForFireBallAppear)
			{
				obj->SetPosition(x /*+ nx * (MARIO_BIG_BBOX_WIDTH + 1.0f)*/, y);
				obj->SetSpeed(nx * FB_SPEED_X, FB_SPEED_Y);
				fb->StartAppear();
				isForFireBallAppear = false;
			}

			if (fb->GetType() == FB_OF_VENUS) 
			{
				if (CheckBB(pLeft, pTop, pRight, pBottom)) 
				{
					SetState(MARIO_STATE_DIE);
					DebugOut(L"\n AAA");
				}
			}
		}
		else if (dynamic_cast<CPlatform*>(obj))
		{
			CPlatform* pf = dynamic_cast<CPlatform*>(obj);
			if (CheckBB(pLeft, pTop, pRight, pBottom))
			{	
				if (pf->getType() == PLATFORM_FOR_ENTER_SECRET_ROOM) {
					isReadyForSecretRoom = true;
				}
				else if (pf->getType() == PLATFORM_TYPE_ONE)
					y -= y + MARIO_BIG_BBOX_HEIGHT - pTop + PushBackPixel;
			}
			else {
					isReadyForSecretRoom = false;
			}
		}
		else if (dynamic_cast<CBreakableBrick*>(obj)) {
			CBreakableBrick* bb = dynamic_cast<CBreakableBrick*>(obj);
			if (CheckBB(pLeft, pTop, pRight, pBottom))
			{
				if (bb->GetState() != COIN) {
					y -= y + MARIO_BIG_BBOX_HEIGHT - pTop + PushBackPixel;
				}	
			}
		}
		else if (dynamic_cast<MovingBrick*>(obj)) {
			if (CheckBB(pLeft, pTop, pRight, pBottom))
			{
				//if (bb->GetState() != COIN) {
				if(level!=MARIO_LEVEL_SMALL)
					y -= y + MARIO_BIG_BBOX_HEIGHT - pTop + PushBackPixel;
				else 
					y -= y + MARIO_SMALL_BBOX_HEIGHT - pTop + PushBackPixel;
				//}
			}
		}
	}
}

void CMario::HandleNormalColision(vector<LPGAMEOBJECT>* coObjects) 
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
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
			if (dynamic_cast<CGoomba*>(e->obj)) 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					if (goomba->GetType() == PARA_GOOMBA) 
					{
						goomba->SetState(PARA_GOOMBA_STATE_WALKING);
						goomba->SetType(GOOMBA);
					}
					else if (goomba->GetType() == GOOMBA)
					{					
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							goomba->StartDisapear();
							
						}						
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
					}
					else if (e->ny < 0) {
						x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;
						vy = 0;
						isReadyToSit = true;
						if (!isReadyToJump)
							isReadyToJump = true;
					
					}
					else if (e->ny > 0)
					{
						x += min_tx * dx + nx * 0.4f;
						y += dy;
						ny = 0;
						isReadyToSit = true;
					}
				}
				else if (plat->getType() == PLATFORM_TYPE_THREE) {
					//ignore this platform
					if (e->ny != 0 || e->nx != 0) {
						x += dx;
						y += dy;
					}
				}
				else if (plat->getType() == PLATFORM_TYPE_ONE) {
					x += min_tx * dx + nx * 0.8f;
					y += min_ty * dy + ny * 0.1f;
					isReadyToJump = true;
					if (state == MARIO_STATE_JUMP)
					{
						state = MARIO_STATE_IDLE;
					}
					if (e->ny != 0) 
					{
						vy = 0;
						/*if (state == MARIO_STATE_FLY)
							state == MARIO_STATE_IDLE;*/
					}
					if (e->nx != 0)vx = 0;
				}
				else if (plat->getType() == PLATFORM_FOR_ENTER_SECRET_ROOM) {
					x += dx;
					y += dy;
					isReadyForSecretRoom = true;
				}
				else if (plat->getType() == 5) {
					if (e->ny != 0) 
					{
						if (isReadyToExit) 
						{
							StartEnter();
							isReadyForSecretRoom = true;
							DebugOut(L"\n Is ready for secret room %d", isReadyForSecretRoom);
						}
						
					}
					
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				float kVx, kVy;
				koopas->GetSpeed(kVx, kVy);

				float kpL, kpT, kpR, kpB;
				koopas->GetBoundingBox(kpL, kpT, kpR, kpB);

				if (e->ny < 0)
				{
					isOnAir = false;
					vy =  -0.3f;
					y += dy;
					if (koopas->GetType() == KOOPA) 
					{
						if (koopas->GetState() == KOOPAS_STATE_WALKING)
						{
							koopas->SetState(KOOPAS_STATE_DIE);
						}
						else if (koopas->GetState() == KOOPAS_STATE_DIE)
						{
							koopas->SetState(KOOPAS_STATE_DIE_MOVING);
						}
						else if (koopas->GetState() == KOOPAS_STATE_DIE_MOVING)
						{
							koopas->SetState(KOOPAS_STATE_DIE);
						}
					}
					else {
						koopas->SetType(KOOPA);
						koopas->SetState(KOOPAS_STATE_WALKING);
					}
					
				}
				if (!isReadyToHold)
				{
					if (e->nx != 0)
					{
						x += min_tx * dx + nx * 0.8f;
						y += min_ty * dy + ny * 0.1f;
						//if koopas is moving then mario die
						if (abs(kVx) > 0)
						{
							if (untouchable == 0)
							{
								if (koopas->GetState() != GOOMBA_STATE_DIE)
								{
									//StartEffect();
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
			else if (dynamic_cast<CCoin*>(e->obj)) {
				CCoin* coin = dynamic_cast<CCoin*>(e->obj);
				x += dx;
				y += dy;
				if (e->nx != 0 || e->ny != 0) 
				{
					coin->SetState(COIN_STATE_DISAPPEAR);
					CGame::GetInstance()->ScoreUp(100);
				}

			}
			else if (dynamic_cast<CBrick*>(e->obj)) {
				x += min_tx * dx + nx * 0.8f;
				y += min_ty * dy + ny * 0.1f;
				isReadyToJump = true;
				if (state == MARIO_STATE_JUMP)
				{
					state = MARIO_STATE_IDLE;
				}
				if (e->ny < 0)
				{
					vy = 0;
				}
				if (e->nx != 0)vx = 0;
			}
			else if (dynamic_cast<CFireBall*>(e->obj)) {
				CFireBall* fb = dynamic_cast<CFireBall*>(e->obj);
				x += dx;
				y += dy;
				/*fb->setIsAppear(true);
				fb->SetPosition(this->x, this->y);
				fb->SetSpeed(0, 0);*/
			}
			else if (dynamic_cast <CQuestionBrick*>(e->obj))
			{
				CQuestionBrick* qb = dynamic_cast <CQuestionBrick*>(e->obj);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (e->nx != 0)
				{
					vx = 0;
				}
				if (e->ny < 0)
				{
					if (state == MARIO_STATE_JUMP) 
					{
						SetState(MARIO_STATE_IDLE);
					}						
					isReadyToJump = true;
					vy = 0;
				}
				else if (e->ny > 0)
				{
					vy += MARIO_FALLING_ACCELERATION *dt;
					isReadyToJump = false;
					if (qb->GetState() != BRICK_STATE_WITH_NO_EFFECT)
					{
						qb->SetState(BRICK_STATE_WITH_NO_EFFECT);
						if (!qb->IsMoving() && !qb->IsUsed()) 
						{
							qb->StartMoving();
						}	
					}
				}
			}
			else if (dynamic_cast<CLeaf_Mushroom*>(e->obj)) 
			{
				CLeaf_Mushroom* item = dynamic_cast<CLeaf_Mushroom*>(e->obj);
				StartEffect();
				x += dx;
				y += dy;
				if (level != MARIO_LEVEL_TAIL) {
					level += LEVEL_DELTA;
				}
				//item->SetEffectScore(true);
				item->SetIsAllowToAppear(false);
			}
			else if (dynamic_cast<CSwitchBlock*>(e->obj)) {
				CSwitchBlock* sb = dynamic_cast<CSwitchBlock*>(e->obj);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				x += dx;
				y += dy;
				if (e->ny < 0) 
				{
					sb->SetState(STATE_IS_ACTIVATED);
				}
					
			}
			else if (dynamic_cast<CBreakableBrick*>(e->obj)) {
				CBreakableBrick* bb = dynamic_cast<CBreakableBrick*>(e->obj);	
				if (bb->GetState() == COIN) 
				{
					x += dx;
					y += dy;
					if (e->nx != 0 || e->ny != 0) {
						bb->SetState(BREAKABLE_BRICK_STATE_DISAPPEAR);
					}
				}			
				else {
					x += min_tx * dx + nx * 0.8f;
					y += min_ty * dy + ny * 0.1f;
					isReadyToJump = true;
					if (state == MARIO_STATE_JUMP)
					{
						state = MARIO_STATE_IDLE;
					}
					if (e->ny < 0)
					{
						vy = 0;
					}
					if (e->ny > 0) {
						vy += MARIO_FALLING_ACCELERATION * dt;
						isReadyToJump = false;
						if (bb->GetState() == SHINING) {
							if (bb->GetType() == SPECIAL)
								bb->SetState(WITHOUT_SHINING);
							else
								bb->SetState(BREAKABLE_BRICK_STATE_DISAPPEAR);
						}
					}
					if (e->nx != 0)vx = 0;
				}
			}
			else if (dynamic_cast<Card*>(e->obj)) {
				Card* card = dynamic_cast<Card*>(e->obj);
				if (e->ny > 0)
				{
					vy += MARIO_FALLING_ACCELERATION * dt;
					isReadyToJump = false;
					isAtTheEnd = true;
					card->StartMoving();
					card->StopRandom();
					CGame::GetInstance()->SetFCard(card->GetID());
				}
			}
			else if (dynamic_cast<MovingBrick*>(e->obj)) {
				/*x += min_tx * dx + nx * 0.1f;
				y += min_ty * dy + ny * 0.1f;*/
				MovingBrick* mb = dynamic_cast<MovingBrick*>(e->obj);
				isReadyToJump = true;
				if (state == MARIO_STATE_JUMP)
				{
					state = MARIO_STATE_IDLE;
				}
				if (e->ny != 0)
				{
					vy = 0;
					mb->SetState(STATE_BRICK_FALLING);
					
				}
				if (e->nx != 0)vx = 0;
				}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isForEffectAppear) 
	{
		CGameObject::Update(dt);

		vx += a * dt;
		vy += ay * dt;
		
		if (y > 800) {
			SetState(MARIO_STATE_DIE);
		}
		//DebugOut(L"\n Is in secret room %d", isInSecretRoom);
		if (state != MARIO_STATE_DIE) {
			UpdateForEachState(dt);
			
			UpdateStateUsingTimeOut();

			if (isEntering)
			{
				if (isInSecretRoom) {
					vx = 0;
					vy = -0.01f;
					y += dy;
					return;
				}
				else {
					vx = 0;
					vy = 0.01f;
					y += dy;
					return;
				}
			}

			HandleNormalColision(coObjects);

			HandleOverlapColision(coObjects);

		}
		else {
			vx = 0;
			x += dx;
			y += dy;
			
			if (!CGame::GetInstance()->IsSwitch()) 
			{
				CGame::GetInstance()->StartSwitchScene();
			}
			if (GetTickCount64() - CGame::GetInstance()->GetTimeStartSwitch() > 1500) 
			{
				CGame::GetInstance()->SetIsSwitch(false);
				CGame::GetInstance()->SetCamPos(0, 0);
				CGame::GetInstance()->SwitchScene(2);
			}
		}

		
	}
	else {
		if (GetTickCount() - EffectTime > 700) {
			EffectTime = 0;
			isForEffectAppear = false;
		}
	}

}

void CMario::SameRenderLogicsForAllLevel(int &ani, int ani_jump_down_right, int ani_jump_down_left,
									int ani_idle_right, int ani_idle_left,
									int ani_stop_right, int ani_stop_left, int ani_walking_right, int ani_walking_left) 
{
		//on the ground and not doing anything
		if (vx == 0) 
		{
			if (state == MARIO_STATE_IDLE ) //if he's on the ground
			{
				if (nx > 0) ani = ani_idle_right;
				else ani = ani_idle_left;
			}
			else 
				ani = ani_idle_right;
		}
		//when mario moving 
		else {
			//if is jumping
			if (!isReadyToJump) 
			{
				RenderByDirection(ani, ani_jump_down_right, ani_jump_down_left);
			}
			else {
				if (vx >= 0 && nx < 0) {
					ani = ani_stop_right;
				}
				else if (vx <= 0 && nx >0) {
					ani = ani_stop_left;
				}
				else if (vx >= 0 && nx > 0) {
					ani = ani_walking_right;
				}
				else if (vx <= 0 && nx < 0) {
					ani = ani_walking_left;
					
				}
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

void CMario::RenderByDirection(int& ani, int ani_right, int ani_left) {
	if (nx > 0)
		ani = ani_right;
	else
		ani = ani_left;
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
	//Render for mario big
	if (level == MARIO_LEVEL_BIG) 
	{
		if (state == MARIO_STATE_SIT) 
		{
			RenderLogicForSittingState(ani, 
				MARIO_ANI_BIG_SIT_RIGHT, 
				MARIO_ANI_BIG_SIT_LEFT);
		}
		else if (state == MARIO_STATE_JUMP && !isHolding) 
		{
			if (isJumpFlying)
			{
				RenderByDirection(ani, MARIO_ANI_BIG_JUMP_FLY_RIGHT, MARIO_ANI_BIG_JUMP_FLY_LEFT);
			}
			else
			RenderLogicForJumpingState(ani, 
				MARIO_ANI_BIG_JUMP_RIGHT, 
				MARIO_ANI_BIG_JUMP_LEFT, 
				MARIO_ANI_BIG_JUMP_DOWN_RIGHT, 
				MARIO_ANI_BIG_JUMP_DOWN_LEFT);
		}
		else if (isKicking) {
			RenderByDirection(ani, MARIO_ANI_BIG_KICK_RIGHT, MARIO_ANI_BIG_KICK_LEFT);
		}
		else if (isRunning)
		{
			RenderLogicForRunningState(ani, MARIO_ANI_BIG_RUN_RIGHT, MARIO_ANI_BIG_RUN_LEFT,
				MARIO_ANI_BIG_WALK_FAST_RIGHT, MARIO_ANI_BIG_WALK_FAST_LEFT);
		}
		else 
		{
			if(!isHolding)
			SameRenderLogicsForAllLevel(ani,
				MARIO_ANI_BIG_JUMP_DOWN_RIGHT, MARIO_ANI_BIG_JUMP_DOWN_LEFT,
				MARIO_ANI_BIG_IDLE_RIGHT, MARIO_ANI_BIG_IDLE_LEFT,
				MARIO_ANI_BIG_STOP_RIGHT, MARIO_ANI_BIG_STOP_LEFT,
				MARIO_ANI_BIG_WALKING_RIGHT, MARIO_ANI_BIG_WALKING_LEFT);
			else {
				if (vx == 0) {
					RenderByDirection(ani, MARIO_ANI_BIG_HOLD_IDLE_RIGHT, MARIO_ANI_BIG_HOLD_IDLE_LEFT);
				}
				else {
					RenderByDirection(ani, MARIO_ANI_BIG_HOLD_MOVING_RIGHT, MARIO_ANI_BIG_HOLD_MOVING_LEFT);
				}
			}
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (state == MARIO_STATE_JUMP && !isHolding) 
		{
			if (isJumpFlying)
			{
				RenderByDirection(ani, MARIO_ANI_SMALL_FLY_JUMP_RIGHT, MARIO_ANI_SMALL_FLY_JUMP_LEFT);
			}
			else
			RenderLogicForJumpingState(ani,
					MARIO_ANI_SMALL_JUMP_RIGHT,
					MARIO_ANI_SMALL_JUMP_LEFT,
					MARIO_ANI_SMALL_JUMP_DOWN_RIGHT,
					MARIO_ANI_SMALL_JUMP_DOWN_LEFT);
		}
		else if (isKicking) {
			RenderByDirection(ani, MARIO_ANI_SMALL_KICK_RIGHT, MARIO_ANI_SMALL_KICK_LEFT);
		}
		else if (isRunning) {
			RenderLogicForRunningState(ani, MARIO_ANI_SMALL_RUN_RIGHT, MARIO_ANI_SMALL_RUN_LEFT,
				MARIO_ANI_SMALL_WALK_FAST_RIGHT, MARIO_ANI_SMALL_WALK_FAST_LEFT);
		}
		else {
			if (!isHolding) {
				SameRenderLogicsForAllLevel(ani,
					MARIO_ANI_SMALL_JUMP_DOWN_RIGHT, MARIO_ANI_SMALL_JUMP_DOWN_LEFT,
					MARIO_ANI_SMALL_IDLE_RIGHT, MARIO_ANI_SMALL_IDLE_LEFT,
					MARIO_ANI_SMALL_STOP_RIGHT, MARIO_ANI_SMALL_STOP_LEFT,
					MARIO_ANI_SMALL_WALKING_RIGHT, MARIO_ANI_SMALL_WALKING_LEFT);
				
			}
			else {
				if (vx == 0) {
					RenderByDirection(ani, MARIO_ANI_SMALL_HOLD_IDLE_RIGHT, MARIO_ANI_SMALL_HOLD_IDLE_LEFT);
				}
				else {
					RenderByDirection(ani, MARIO_ANI_SMALL_HOLD_MOVING_RIGHT, MARIO_ANI_SMALL_HOLD_MOVING_LEFT);
				}
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
			RenderLogicForJumpingState(ani,
				MARIO_ANI_TAIL_JUMP_RIGHT,
				MARIO_ANI_TAIL_JUMP_LEFT,
				MARIO_ANI_TAIL_JUMP_DOWN_RIGHT,
				MARIO_ANI_TAIL_JUMP_DOWN_LEFT);
		}
		else if (isFlying) {
			RenderByDirection(ani, MARIO_ANI_TAIL_JUMP_FLY_RIGHT, MARIO_ANI_TAIL_JUMP_FLY_LEFT);
		}
		else if (isKicking) {
			RenderByDirection(ani, MARIO_ANI_TAIL_KICK_RIGHT, MARIO_ANI_TAIL_KICK_LEFT);
		}
		else if (state==MARIO_STATE_JUMP_FLY) {
			RenderLogicForSittingState(ani, MARIO_ANI_TAIL_JUMP_FLY_RIGHT, MARIO_ANI_TAIL_JUMP_FLY_LEFT);
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
		else if (isEntering || isExiting) {
			ani = MARIO_ANI_TAIL_ENTER_SECRET_ROOM;
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


	if (isForEffectAppear) 
	{
		ani = MARIO_ANI_EFFECT;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;

	if (state == MARIO_STATE_DIE) {
		ani = MARIO_ANI_DIE;
	}
	
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
		if (isRunning)
			a = 0.00015f;
		break;
	case MARIO_STATE_WALKING_LEFT:
		a = -MARIO_ACCELERATION;
		nx = -1;
		if (isRunning)
			a = -0.00015f;
		break;
	case MARIO_STATE_JUMP:
		if (level == MARIO_LEVEL_TAIL) 
		{
			isOnAir = true;
		}
		vy = -MARIO_JUMP_SPEED_MAX;
		break;
	case MARIO_STATE_JUMP_FLY:
		if (level == MARIO_LEVEL_TAIL)
		{
			isOnAir = true;
		}
		vy = -MARIO_JUMP_SPEED_MAX;
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
		if (vx != 0)
			vx = 0;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_STATE_FLY:
		vy = -MARIO_FLY_SPEED;
		isOnAir = true;
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{	
	left = x;
	top = y;
	if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL) 
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
		if (state == MARIO_STATE_SIT)
		{
			bottom = y + MARIO_HEIGHT_SIT_BBOX ;
		}
		else 
		{
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}		
	}
	else
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		if (state == MARIO_STATE_SIT)
		{
			bottom = y + MARIO_HEIGHT_SIT_BBOX;
		}
		else {
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}	

	if (state == MARIO_STATE_DIE) {
		left = top = right = bottom = 0;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	nx = 1;
	a = MARIO_ACCELERATION;
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

