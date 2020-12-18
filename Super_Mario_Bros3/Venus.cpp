#include "Venus.h"

CVenus::CVenus(int type)
{
	nx = 1;
	SetState(VENUS_RED);
	this->type = type;
	vx = 0;
	vy = 0;
	nx = RIGHT;
	ny = UP;
}



void CVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	

	if (isAllowToMove) 
	{
		if (GetTickCount() - TimeMovingY > 1600)
		{
			StartBlock();
		}
	}
	if (!isAllowToMove) 
	{
		if (GetTickCount() - TimeBlockMoving > 3000) 
		{
			StartMoving();
		}
	}

	if (isMoving)
	{
		if (ReadyToUp > 0) {
			vy = -0.02f;
		}
		else
			vy = 0.02f;
	}
	else {
		vy = 0;
	}
		
	for (size_t i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CMario*>(obj)) {
			if (obj->x <= this->x) nx = LEFT;
			else nx = RIGHT;

			if (obj->y <= this->y) ny = UP;
			else ny = DOWN;
			
		}
	}
	
}


void CVenus::Render()
{
	int ani=0;
	if ( type == VENUS_RED) 
	{
		//right
		if (nx > 0) 
		{
			//down
			if (ny > 0) {
				ani = VENUS_RED_ANI_DOWN_RIGHT;
			}
			//up
			else
				ani = VENUS_RED_ANI_UP_RIGHT;
			
		}
		//left
		else
		{
			//down
			if (ny > 0) {
				ani = VENUS_RED_ANI_DOWN_LEFT;
			}
			//up
			else
				ani = VENUS_RED_ANI_UP_LEFT;
		}
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CVenus::SetState(int state)
{
	CGameObject::SetState(state);
}

void CVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}