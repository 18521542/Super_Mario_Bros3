#include "QuestionBrick.h"
#include "Utils.h"
CQuestionBrick::CQuestionBrick()
{
	state = BRICK_STATE_WITH_EFFECT;
	vx = 0;
	vy = 0;
}
void CQuestionBrick::Render()
{
	if(state == BRICK_STATE_WITH_EFFECT)
		animation_set->at(0)->Render(x, y);
	else 
		animation_set->at(1)->Render(x, y);
	//animation_set->at(1)->Render(x, y);
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;
	if (isMoving) {
		vy += 0.000451f * dt;
	}
	else {
		vy = 0;
	}

	if (GetTickCount() - TimeStartMove > 421) {
		TimeStartMove = 0;
		isMoving = false;
	}
}