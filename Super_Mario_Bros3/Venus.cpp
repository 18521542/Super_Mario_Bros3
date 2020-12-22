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

	//fireball = new CFireBall();
}



void CVenus::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (!isInitFB) 
	{
		this->fireball = new CFireBall(1);
		isInitFB = true;
		this->fireball->x = x;
		this->fireball->y = y;
		this->fireball->vx = 0;
		this->fireball->vy = 0;
	}

	//this->fireball->Update(dt, coObjects);

	
	if (ReadyToUp == -1 && !isMoving ) 
	{
		if (!HasFired) 
		{			
			this->fireball->StartAppear();
			this->fireball->vx = this->nx * FB_OF_VENUS_SPEED;
			this->fireball->vy = this->ny * FB_OF_VENUS_SPEED;
			this->HasFired = true;
		}
	}
	else {
		this->fireball->x = x + TO_CENTER;
		this->fireball->y = y + TO_CENTER;
		this->fireball->SetDir(this->nx, this->ny);
		this->fireball->setIsAppear(false);
		HasFired = false;
	}
	this->fireball->Update(dt, coObjects);

	
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
	else if (type == VENUS_GREEN) {
		//right
		if (nx > 0)
		{
			//down
			if (ny > 0) {
				ani = VENUS_GREEN_ANI_DOWN_RIGHT;
			}
			//up
			else
				ani = VENUS_GREEN_ANI_UP_RIGHT;

		}
		//left
		else
		{
			//down
			if (ny > 0) {
				ani = VENUS_GREEN_ANI_DOWN_LEFT;
			}
			//up
			else
				ani = VENUS_GREEN_ANI_UP_LEFT;
		}
	}

	animation_set->at(ani)->Render(x, y);

	this->fireball->Render();

	RenderBoundingBox();
}


void CVenus::SetState(int state)
{
	CGameObject::SetState(state);
}

void CVenus::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}