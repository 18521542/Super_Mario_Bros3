#include "Effect.h"
#include "Utils.h"
Effect::Effect() 
{
	type = TYPE_SCORE;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_EFFECT));
	state = STATE_SCORE_100;
}
void Effect::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = top = right = bottom = 0;
}
void Effect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	DebugOut(L"\nis Appear  %d", isAppear);
	if (isAppear) 
	{
		//DebugOut(L"\nTime %d", GetTickCount64() - StartAppearTime);
		if (GetTickCount64() - StartAppearTime > 1000) {
			StartAppearTime = 0;
			isAppear = false;
		}

		CGameObject::Update(dt, coObjects);
		vy = -EFFECT_VY;
		y += dy;
	}
	else {
		return;
	}
}
void Effect::Render() {
	int ani = -1;
	
	if (type == TYPE_SCORE) {
		if (state == STATE_SCORE_100) ani = EFFECT_ANI_SCORE_100;
		else if (state == STATE_SCORE_1000) ani = EFFECT_ANI_SCORE_1000;
	}

	//DebugOut(L"\nRender is effect at %d", isAppear);
	if (isAppear)
		animation_set->at(ani)->Render(x, y);
}