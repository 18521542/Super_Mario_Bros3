#pragma once
#include "GameObject.h"

#define ANI_SET_EFFECT	12
#define EFFECT_VY		0.05f	

#define TYPE_SCORE		1
#define TYPE_DEAD		2

#define STATE_SCORE_100		100
#define STATE_SCORE_200		200
#define STATE_SCORE_400		400
#define STATE_SCORE_800		800
#define STATE_SCORE_1000	1000
#define STATE_SCORE_2000	2000
#define STATE_SCORE_4000	4000
#define STATE_SCORE_8000	8000

#define EFFECT_ANI_SCORE_100	0
#define EFFECT_ANI_SCORE_200	1
#define EFFECT_ANI_SCORE_400	2
#define EFFECT_ANI_SCORE_800	3
#define EFFECT_ANI_SCORE_1000	4
#define EFFECT_ANI_SCORE_2000	5
#define EFFECT_ANI_SCORE_4000	6
#define EFFECT_ANI_SCORE_8000	7

class Effect : public CGameObject 
{
	bool hasAppear = false;
	int type;
	bool isAppear = false;
	ULONGLONG StartAppearTime;
public:
	Effect();
	void SetType(int type) {
		this->type = type;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void StartAppear() { StartAppearTime = GetTickCount64(); isAppear = true; hasAppear = true; }
	bool HasAppear() { return this->hasAppear; }
	bool IsAppear() { return this->isAppear; }
};