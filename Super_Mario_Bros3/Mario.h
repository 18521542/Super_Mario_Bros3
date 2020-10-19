#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_ACCELERATION		0.5f

//define state - xxx
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_KICK			400
#define MARIO_STATE_TALE			500
#define MARIO_STATE_THROW_FIRE		600
#define MARIO_STATE_SWIM			700
#define MARIO_STATE_DIE				800

//define animation - xx
#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_BIG_WALKING_RIGHT		4
#define MARIO_ANI_BIG_WALKING_LEFT		5

#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT		3
#define MARIO_ANI_SMALL_WALKING_RIGHT	6
#define MARIO_ANI_SMALL_WALKING_LEFT	7

#define MARIO_ANI_FIRE_IDLE_RIGHT		9 
#define MARIO_ANI_FIRE_IDLE_LEFT		10
#define MARIO_ANI_FIRE_WALKING_RIGHT	11
#define MARIO_ANI_FIRE_WALKING_LEFT		12

#define MARIO_ANI_TAIL_IDLE_RIGHT		13
#define MARIO_ANI_TAIL_IDLE_LEFT		14
#define MARIO_ANI_TAIL_WALKING_RIGHT	15
#define MARIO_ANI_TAIL_WALKING_LEFT		16

#define MARIO_ANI_FROG_IDLE_RIGHT		17
#define MARIO_ANI_FROG_IDLE_LEFT		18
#define MARIO_ANI_FROG_WALKING_RIGHT	19
#define MARIO_ANI_FROG_WALKING_LEFT		20

#define MARIO_ANI_HAMMER_IDLE_RIGHT		21
#define MARIO_ANI_HAMMER_IDLE_LEFT		22
#define MARIO_ANI_HAMMER_WALKING_RIGHT	23
#define MARIO_ANI_HAMMER_WALKING_LEFT	24

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

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return this->level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};