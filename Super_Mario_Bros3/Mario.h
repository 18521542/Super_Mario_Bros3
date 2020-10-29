#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED_START	0.001f 
#define MARIO_WALKING_SPEED_MAX		0.15f
#define MARIO_ACCELERATION			0.0003f
#define MARIO_WALKING_SPEED_MIN		0.02f
#define MARIO_ACCELERATION_JUMP		0.0005f
//0.1f
#define MARIO_JUMP_SPEED_MAX		0.3f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.001f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

//time
#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_USING_TAIL_TIME 200

//define state - xxx
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_SLOWINGDOWN		1
#define MARIO_STATE_SIT				2
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_KICK			400
#define MARIO_STATE_TALE			500
#define MARIO_STATE_THROW_FIRE		600
#define MARIO_STATE_SWIM			700
#define MARIO_STATE_DIE				800
#define	MARIO_STATE_FALING_DOWN		900
#define MARIO_STATE_START_USING_TAIL		3

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

#define MARIO_ANI_FROG_IDLE_RIGHT		17
#define MARIO_ANI_FROG_IDLE_LEFT		18
#define MARIO_ANI_FROG_WALKING_RIGHT	19
#define MARIO_ANI_FROG_WALKING_LEFT		20

#define MARIO_ANI_HAMMER_IDLE_RIGHT		21
#define MARIO_ANI_HAMMER_IDLE_LEFT		22
#define MARIO_ANI_HAMMER_WALKING_RIGHT	23
#define MARIO_ANI_HAMMER_WALKING_LEFT	24
#define MARIO_ANI_HAMMER_SIT_RIGHT		55
#define MARIO_ANI_HAMMER_SIT_LEFT		56
#define MARIO_ANI_HAMMER_JUMP_RIGHT		57
#define MARIO_ANI_HAMMER_JUMP_LEFT		58
#define MARIO_ANI_HAMMER_JUMP_DOWN_RIGHT	59
#define MARIO_ANI_HAMMER_JUMP_DOWN_LEFT		60
#define MARIO_ANI_HAMMER_STOP_RIGHT		61
#define MARIO_ANI_HAMMER_STOP_LEFT		62

#define MARIO_ANI_DIE				8

//define level
#define	MARIO_LEVEL_SMALL	1111
#define	MARIO_LEVEL_BIG		2222
#define	MARIO_LEVEL_FIRE	3333
#define	MARIO_LEVEL_FROG	4444
#define	MARIO_LEVEL_HAMMER	5555
#define	MARIO_LEVEL_TAIL	6666

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15




class CMario : public CGameObject
{
	float a;	// vx = vx + a*dt
	float ay;	//vy = ay*dt;

	bool isReadyToSit = true;
	bool isReadyToJump = true;

	bool isReadyToUseTail = false;
	bool isStartUsingTail = false;

	bool isShootingFireBall = false;
	bool isHolding = false;
	bool isReadyToHold = false;

	int level;
	int untouchable;
	bool isUsingTail = false;
	DWORD untouchable_start;
	DWORD using_tail_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SameRenderLogicsForAllLevel(int &ani, 
		int ani_idle_right, int ani_idle_left, int ani_jump_down_right, int ani_jump_down_left,
		int ani_stop_right, int ani_stop_left, int ani_walking_right, int ani_walking_left);
	void RenderLogicForSittingState(int& ani, int ani_sit_right, int ani_sit_left);
	void RenderLogicForJumpingState(int& ani, int ani_jump_up_right, int ani_jump_up_left, int ani_jump_down_right, int ani_jump_down_left);
	
	//timer
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartUsingTail() { using_tail_start = GetTickCount(); isUsingTail = true; }

	void Reset();

	//get
	bool IsReadyToHold() { return isReadyToHold; }
	bool IsReadyToJump() {return isReadyToJump;}
	bool IsReadyToSit() {	return isReadyToSit; }
	bool IsStartUsingTail() { return this->isStartUsingTail; }
	bool IsUsingTail() { return this->isUsingTail; }
	bool IsShootingFireBall() { return this->isShootingFireBall; }
	int GetLevel() { return this->level; }
	
	//set
	void SetIsReadyToHold(bool hold) { this->isReadyToHold = hold; }

	void SetIsStartUsingTail(bool tail) { this->isStartUsingTail = tail; }

	void setIsReadyToJump(bool jump) {isReadyToJump = jump;}

	void setIsReadyToSit(bool use) { this->isReadyToSit = use; }

	void SetState(int state);

	void SetLevel(int l) { level = l; }

	void SetIsShootingFireBall(bool shoot) { this->isShootingFireBall = shoot; }

	//bounding box
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};