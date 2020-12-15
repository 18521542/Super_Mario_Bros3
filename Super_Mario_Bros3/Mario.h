#pragma once
#include "GameObject.h"
#include "Object.h"
#define MARIO_WALKING_SPEED_START		0.001f 

#define MARIO_WALKING_SPEED_MAX			0.1f

#define MARIO_ACCELERATION				0.0003f

#define MARIO_WALKING_SPEED_MIN			0.01f

#define MARIO_JUMPING_ACCELERATION		0.0007f

#define MARIO_FLY_SPEED					0.1f	

#define MARIO_JUMP_SPEED_MAX			0.2f

#define MARIO_JUMP_DEFLECT_SPEED		0.2f

#define MARIO_GRAVITY					0.001f

#define MARIO_FALLING_ACCELERATION		0.005f

#define MARIO_DIE_DEFLECT_SPEED			0.5f

#define MARIO_RUN_SPEED_MAX				0.3f

#define MARIO_FALLING_SPEED				0.00004f

//time
#define MARIO_UNTOUCHABLE_TIME			5000
#define MARIO_USING_TAIL_TIME			300

#define MARIO_FOR_TAIL_APPEAR_TIME		50

#define MARIO_SHOOTING_TIME				200
#define MARIO_FLYING_TIME				5000
#define MARIO_KICKING_TIME				300

//define state - xxx
#define MARIO_STATE_IDLE				0
#define MARIO_STATE_SIT					2
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_JUMP				300
#define MARIO_STATE_KICK				400
#define MARIO_STATE_TALE				500
#define MARIO_STATE_THROW_FIRE			600
#define MARIO_STATE_SWIM				700
#define MARIO_STATE_DIE					800
#define	MARIO_STATE_FALING_DOWN			900
#define MARIO_STATE_START_USING_TAIL	3
#define MARIO_STATE_RUN					1000
#define MARIO_STATE_JUMP_FLY			1100
#define MARIO_STATE_FLY					1200


//define animation 
#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_BIG_WALKING_RIGHT		4
#define MARIO_ANI_BIG_WALKING_LEFT		5
#define MARIO_ANI_BIG_STOP_RIGHT		27
#define MARIO_ANI_BIG_STOP_LEFT			28
#define	MARIO_ANI_BIG_JUMP_RIGHT		29
#define	MARIO_ANI_BIG_JUMP_LEFT			30
#define	MARIO_ANI_BIG_JUMP_DOWN_RIGHT	31
#define	MARIO_ANI_BIG_JUMP_DOWN_LEFT	32
#define	MARIO_ANI_BIG_SIT_RIGHT			33
#define	MARIO_ANI_BIG_SIT_LEFT			34
#define MARIO_ANI_BIG_RUN_RIGHT			67	
#define MARIO_ANI_BIG_RUN_LEFT			68
#define MARIO_ANI_BIG_WALK_FAST_RIGHT	69
#define MARIO_ANI_BIG_WALK_FAST_LEFT	70
#define MARIO_ANI_BIG_JUMP_FLY_RIGHT	90
#define MARIO_ANI_BIG_JUMP_FLY_LEFT		89

#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT		3
#define MARIO_ANI_SMALL_WALKING_RIGHT	6
#define MARIO_ANI_SMALL_WALKING_LEFT	7
#define MARIO_ANI_SMALL_STOP_LEFT		25
#define MARIO_ANI_SMALL_STOP_RIGHT		26
#define	MARIO_ANI_SMALL_JUMP_RIGHT		35
#define	MARIO_ANI_SMALL_JUMP_LEFT		36
#define	MARIO_ANI_SMALL_JUMP_DOWN_RIGHT	37
#define	MARIO_ANI_SMALL_JUMP_DOWN_LEFT	38
#define	MARIO_ANI_SMALL_WALK_FAST_RIGHT	71
#define	MARIO_ANI_SMALL_WALK_FAST_LEFT	72
#define	MARIO_ANI_SMALL_RUN_RIGHT		73
#define	MARIO_ANI_SMALL_RUN_LEFT		74

#define MARIO_ANI_FIRE_IDLE_RIGHT		9 
#define MARIO_ANI_FIRE_IDLE_LEFT		10
#define MARIO_ANI_FIRE_WALKING_RIGHT	11
#define MARIO_ANI_FIRE_WALKING_LEFT		12
#define MARIO_ANI_FIRE_SIT_RIGHT		39
#define MARIO_ANI_FIRE_SIT_LEFT			40
#define MARIO_ANI_FIRE_JUMP_RIGHT		41
#define MARIO_ANI_FIRE_JUMP_LEFT		42
#define MARIO_ANI_FIRE_JUMP_DOWN_RIGHT	43
#define MARIO_ANI_FIRE_JUMP_DOWN_LEFT	44
#define MARIO_ANI_FIRE_STOP_RIGHT		45
#define MARIO_ANI_FIRE_STOP_LEFT		46
#define MARIO_ANI_FIRE_SHOOT_RIGHT		65
#define MARIO_ANI_FIRE_SHOOT_LEFT		66
#define MARIO_ANI_FIRE_WALK_FAST_RIGHT	79
#define MARIO_ANI_FIRE_WALK_FAST_LEFT	80
#define MARIO_ANI_FIRE_RUN_RIGHT		81	
#define MARIO_ANI_FIRE_RUN_LEFT			82

#define MARIO_ANI_TAIL_IDLE_RIGHT		13
#define MARIO_ANI_TAIL_IDLE_LEFT		14
#define MARIO_ANI_TAIL_WALKING_RIGHT	15
#define MARIO_ANI_TAIL_WALKING_LEFT		16
#define MARIO_ANI_TAIL_SIT_RIGHT		47
#define MARIO_ANI_TAIL_SIT_LEFT			48
#define MARIO_ANI_TAIL_JUMP_RIGHT		49
#define MARIO_ANI_TAIL_JUMP_LEFT		50
#define MARIO_ANI_TAIL_JUMP_DOWN_RIGHT	51
#define MARIO_ANI_TAIL_JUMP_DOWN_LEFT	52
#define MARIO_ANI_TAIL_STOP_RIGHT		53
#define MARIO_ANI_TAIL_STOP_LEFT		54
#define MARIO_ANI_TAIL_USETAIL_LEFT		63
#define MARIO_ANI_TAIL_USETAIL_RIGHT	64
#define	MARIO_ANI_TAIL_WALK_FAST_RIGHT	75
#define	MARIO_ANI_TAIL_WALK_FAST_LEFT	76
#define	MARIO_ANI_TAIL_RUN_RIGHT		77
#define	MARIO_ANI_TAIL_RUN_LEFT			78	
#define MARIO_ANI_TAIL_JUMP_FLY_RIGHT	83
#define MARIO_ANI_TAIL_JUMP_FLY_LEFT	84
#define MARIO_ANI_TAIL_FALLING_RIGHT	85
#define MARIO_ANI_TAIL_FALLING_LEFT		86
#define MARIO_ANI_TAIL_KICK_RIGHT		87
#define MARIO_ANI_TAIL_KICK_LEFT		88	


#define MARIO_ANI_FROG_IDLE_RIGHT		17
#define MARIO_ANI_FROG_IDLE_LEFT		18
#define MARIO_ANI_FROG_WALKING_RIGHT	19
#define MARIO_ANI_FROG_WALKING_LEFT		20

#define MARIO_ANI_HAMMER_IDLE_RIGHT			21
#define MARIO_ANI_HAMMER_IDLE_LEFT			22
#define MARIO_ANI_HAMMER_WALKING_RIGHT		23
#define MARIO_ANI_HAMMER_WALKING_LEFT		24
#define MARIO_ANI_HAMMER_SIT_RIGHT			55
#define MARIO_ANI_HAMMER_SIT_LEFT			56
#define MARIO_ANI_HAMMER_JUMP_RIGHT			57
#define MARIO_ANI_HAMMER_JUMP_LEFT			58
#define MARIO_ANI_HAMMER_JUMP_DOWN_RIGHT	59
#define MARIO_ANI_HAMMER_JUMP_DOWN_LEFT		60
#define MARIO_ANI_HAMMER_STOP_RIGHT			61
#define MARIO_ANI_HAMMER_STOP_LEFT			62

#define MARIO_ANI_DIE						8

#define MARIO_ANI_EFFECT					91

//define level
#define	MARIO_LEVEL_SMALL	1111
#define	MARIO_LEVEL_BIG		2222
#define	MARIO_LEVEL_FIRE	6666
#define	MARIO_LEVEL_FROG	4444
#define	MARIO_LEVEL_HAMMER	5555
#define	MARIO_LEVEL_TAIL	3333

#define LEVEL_DELTA			1111

//Bounding box
#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_TAIL_BBOX_WIDTH	21
#define TAIL_BBOX_WIDTH			8
#define TAIL_BBOX_HEIGHT		2
#define MARIO_HEIGHT_SIT_BBOX	19

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15


class CMario : public CGameObject
{
	float a;	// vx = vx + a*dt
	float ay;	//vy = ay*dt;

	//sit
	bool isReadyToSit = true;

	bool isAllowtoStop = false;

	//hight-jump
	bool isReadyToJump = true;

	//using tail
	ULONGLONG using_tail_start;
	ULONGLONG tail_appear;
	bool isUsingTail = false;
	bool isForTailAppear = false;

	//shoot
	ULONGLONG shooting_start;
	bool isShootingFireBall = false;
	bool isForFireBallAppear = false;

	//hold
	bool isHolding = false;
	bool isReadyToHold = false;

	//run
	bool isReadyToRun = false;
	bool isRunning = false;

	//fly
	ULONGLONG StartFly;
	bool isFlying = false;
	bool isReadyToFly = false;

	//tail - falling
	bool isOnAir = false;
	bool isFalling = false;

	//JumpFly
	bool isJumpFlying = false;
	bool isReadyToJumpFly = false;

	//kick
	ULONGLONG StartKick;
	bool isKicking = false;

	int level;
	int untouchable;
	
	//time
	ULONGLONG untouchable_start;

	bool isForEffectAppear = false;
	ULONGLONG EffectTime;


	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CMario(float x , float y );
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	

	//Render
	virtual void Render();

	void SameRenderLogicsForAllLevel(int &ani, 
		int ani_idle_right, int ani_idle_left, int ani_jump_down_right, int ani_jump_down_left,
		int ani_stop_right, int ani_stop_left, int ani_walking_right, int ani_walking_left);

	void RenderLogicForSittingState(int& ani, int ani_sit_right, int ani_sit_left);

	void RenderLogicForJumpingState(int& ani, int ani_jump_up_right, int ani_jump_up_left, int ani_jump_down_right, int ani_jump_down_left);

	void RenderLogicForRunningState(int& ani, int ani_run_right, int ani_run_left, int ani_walk_fast_right, int ani_walk_fast_left);
	
	//timer
	void StartUntouchable() { untouchable_start = GetTickCount64(); untouchable = 1;}
	void StartUsingTail() { using_tail_start = GetTickCount64(); tail_appear = GetTickCount64();  isUsingTail = true; isForTailAppear = true; }
	void StartShootingFireBall() { shooting_start = GetTickCount64(); isShootingFireBall = true; isForFireBallAppear = true; }
	void StartFlying() { StartFly = GetTickCount64(); isFlying = true; }
	void StartKicking() { StartKick = GetTickCount64(); isKicking = true; }
	void StartEffect() { EffectTime = GetTickCount64(); isForEffectAppear = true; }
	ULONGLONG GetStartFly() { return this->StartFly; }

	//from beginning
	void Reset();

	int GetLevel() { return this->level; }

	void SetState(int state);

	void SetLevel(int l) { level = l; }

	//get
	bool IsReadyToHold() { return isReadyToHold; }

	bool IsHolding() { return this->isHolding; }

	bool IsReadyToJump() {return isReadyToJump;}

	bool IsReadyToSit() {	return isReadyToSit; }

	bool IsReadyToRun() { return isReadyToRun; }

	bool IsRunning() { return isRunning; }

	bool IsUsingTail() { return this->isUsingTail; }

	bool IsShootingFireBall() { return this->isShootingFireBall; }

	bool IsReadyToJumpFly() { return this->isReadyToJumpFly; }

	bool IsJumpFlying() { return this->isJumpFlying; }

	bool IsReadyToFly() { return this->isReadyToFly; }

	bool IsFlying() { return this->isFlying; }

	bool IsOnAir() { return this->isOnAir; }

	bool IsFalling() { return this->isFalling; }

	bool IsForTailAppear() { return this->isForTailAppear; }

	bool IsAllowToStop() { return this->isAllowtoStop; }
	
	//set
	void SetIsReadyToHold(bool hold) { this->isReadyToHold = hold; }

	void setIsReadyToJump(bool jump) {isReadyToJump = jump;}

	void setIsReadyToSit(bool sit) { this->isReadyToSit = sit; }

	void setIsReadyToRun(bool run) { this->isReadyToRun = run; }

	void setIsRunning(bool run) { this->isRunning = run; }

	void SetIsShootingFireBall(bool shoot) { this->isShootingFireBall = shoot; }

	void setIsJumpFlying(bool jumpfly) { this->isJumpFlying = jumpfly; }

	void setIsReadyToJumpFlying(bool jumpfly) { this->isReadyToJumpFly=jumpfly; }
	
	void setIsFlying(bool fly) { this->isFlying = fly; }

	void setIsReadyToFly(bool fly) { this->isReadyToFly = fly; }

	void setIsOnAir(bool onair) { this->isOnAir = onair; }

	void setIsFalling(bool falling) { this->isFalling = falling; }

	void setIsForTailAppear(bool isforTailAppear) { this->isForTailAppear = isforTailAppear; }

	void setIsAllowToStop(bool stop) { this->isAllowtoStop = stop; }

	void UpdateStateUsingTimeOut();

	void UpdateForEachState(DWORD dt);

	void HandleOverlapColision(vector<LPGAMEOBJECT>* coObjects);

	void HandleNormalColision(vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};