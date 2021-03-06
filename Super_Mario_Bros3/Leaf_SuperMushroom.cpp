#include "Leaf_SuperMushroom.h"
#include "Utils.h"
#include "PlayScene.h"
#include "BreakableBrick.h"

CLeaf_Mushroom::CLeaf_Mushroom(int _type)
{
	type = _type;
	vy = 0;
	vx = 0;
	state = LEAF_STATE_RIGHT;
	nx = 1;
	isAllowToAppear = false;
}
void CLeaf_Mushroom::Render()
{
	if (isAllowToAppear) 
	{
		if (type == MUSHROOM)
		{
			animation_set->at(MUSHROOM_ANI)->Render(x, y);
		}
		else if (type == LEAF)
		{
			if (state == LEAF_STATE_LEFT)
				animation_set->at(LEAF_ANI_LEFT)->Render(x, y);
			else if (state == LEAF_STATE_RIGHT)
				animation_set->at(LEAF_ANI_RIGHT)->Render(x, y);
		}
		else if (type == GREEN_MUSHROOM) {
			animation_set->at(GREEN_MUSHROOM_ANI)->Render(x, y);
		}
	}	

}

void CLeaf_Mushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isAllowToAppear) {
		if (type == LEAF) {
			l = x;
			t = y;
			r = x + LEAF_WIDTH;
			b = y + LEAF_HEIGHT;
		}
		else if (type == MUSHROOM || type == GREEN_MUSHROOM) {
			l = x;
			t = y;
			r = x + MUSHROOM_WIDTH;
			b = y + MUSHROOM_HEIGHT;
		}
	}
	else {
		l = t = r = b = 0;
	}
	
}

#define MUSHROOM_APPEAR_TIME_LIMIT	1000
#define MUSHROOM_APPEAR_TIME	500

#define FIRST_APPEAR_SPEED_X_OF_LEAF	0.4f
#define SPEED_Y_DOWN_OF_LEAF	0.035f
#define LEAF_ACCELERATION	0.0003f
#define MAX_MIN_SPEED	0.12f
#define LEAF_UP_TIME	100
#define FIRST_APPEAR_ACCELERATION_OF_MUSHROOM	0.00005f
#define FIRST_APPEAR_TIME	270
#define SPEED_MUSHROOM	0.05f
void CLeaf_Mushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//decide which will appear : leaf or mushroom, and when will appear
	if (type != GREEN_MUSHROOM) 
	{
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		{
			this->type = MUSHROOM;
			if (mario->x < this->x && !isUsed) {
				this->nx = 1;
			}
			else if (mario->x >= this->x && !isUsed) {
				this->nx = -1;
			}
		}
		else
			this->type = LEAF;
		for (size_t i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			float pLeft, pTop, pRight, pBottom;
			obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
			if (dynamic_cast<CQuestionBrick*>(obj))
			{
				float qX, qY;
				CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(obj);
				qb->GetPosition(qX, qY);
				if (qb->IsUsed() && (this->x == qX || this->y == qY) && !this->isUsed)
				{
					isUsed = true;
					if (this->type == MUSHROOM)
					{
						StartWaitingToMove();
					}
					else {
						LeafStartMoving();
						isAllowToAppear = true;
					}
				}
			}
		}
	}
	else {
		if (mario->x < this->x && !isUsed) {
			this->nx = 1;
		}
		else if (mario->x >= this->x && !isUsed) {
			this->nx = -1;
		}
		for (size_t i = 0; i < coObjects->size(); i++) 
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			if (dynamic_cast<CQuestionBrick*>(obj))
			{
				float qX, qY;
				CQuestionBrick* qb = dynamic_cast<CQuestionBrick*>(obj);
				qb->GetPosition(qX, qY);
				if (qb->IsUsed() && (this->x == qX || this->y == qY) && !this->isUsed)
				{
					isUsed = true;
					StartWaitingToMove();
				}
			}
		}
	}
	
	if ((GetTickCount64() - TimeForMushroomAppear) < MUSHROOM_APPEAR_TIME_LIMIT
		&& (GetTickCount64() - TimeForMushroomAppear) > MUSHROOM_APPEAR_TIME)
	{
		isAllowToAppear = true;
		MushroomStartMoving();
	}
	
	if (isLeafMoving && type==LEAF) 
	{
		if (GetTickCount64() - StartEffectTime < LEAF_UP_TIME) {
			vy = -FIRST_APPEAR_SPEED_X_OF_LEAF;
		}
		else
		{
			vy = SPEED_Y_DOWN_OF_LEAF;
			vx += nx * LEAF_ACCELERATION * dt;
			if (abs(vx) > MAX_MIN_SPEED)
			{
				nx = -nx;
			}
		}
		y += dy;
		x += dx;
		if (nx > 0)
		{
			SetState(LEAF_STATE_RIGHT);
		}
		else if (nx < 0)
			SetState(LEAF_STATE_LEFT);
	}
	else if ((type == MUSHROOM|| type == GREEN_MUSHROOM) && isMushroomMoving)
	{
		if (GetTickCount64() - StartEffectTime < FIRST_APPEAR_TIME) {
			vy += -FIRST_APPEAR_ACCELERATION_OF_MUSHROOM * dt;
		}
		else
		{
			vy += GRAVITY * dt;
			vx = nx * SPEED_MUSHROOM;
		}
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

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


			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<CPlatform*>(e->obj)) 
				{
					CPlatform* plat = dynamic_cast<CPlatform*>(e->obj);
					x += dx;
					y += min_ty * dy + ny * 0.4f;
					if (e->ny < 0) {
						vy = 0;				
					}
					if (e->nx < 0) {
						this->nx = -this->nx;
						//vx = this->nx * vx;
					}
				}
				else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
					CQuestionBrick* plat = dynamic_cast<CQuestionBrick*>(e->obj);
					
					if (e->ny != 0) {
						vy = 0;
					}
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
				}
				else if (dynamic_cast<CBreakableBrick*>(e->obj)) {
					CBreakableBrick* plat = dynamic_cast<CBreakableBrick*>(e->obj);
					
					if (e->ny != 0) {
						vy = 0;
					}
					if (e->nx != 0) {
						this->nx = -this->nx;
					}
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
				}
				//else if (dynamic_cast<MovingBrick*>(e->obj)) {
				//	/*x += dx;
				//	y += dy;*/
				//}
				//else {
				//	x += dx;
				//	y += dy;
				//}
			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}


