#include "QuestionBrick.h"
#include "Utils.h"
CQuestionBrick::CQuestionBrick(float y,float x)
{
	state = BRICK_STATE_WITH_EFFECT;
	vx = 0;
	vy = 0;
	startY = y;
}
void CQuestionBrick::Render()
{
	if(state == BRICK_STATE_WITH_EFFECT)
		animation_set->at(QUESTION_ANI_MOVE)->Render(x, y);
	else 
		animation_set->at(QUESTION_ANI_NOT_MOVE)->Render(x, y);
	//CoinEffect->Render();
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
	if (isMoving) 
	{
		vy += GRAVITY * dt;
	}
	else {
		vy = 0;
	}

	if (isUsed && vy==0) 
	{
		if (y != startY) 
		{
			y = startY;
			SetState(10);
		}
	}

	if (GetTickCount() - TimeStartMove > MOVING_TIME) {
		TimeStartMove = 0;
		isMoving = false;
		isFinishMoving = true;
	}

	//CoinEffect->Update();
}