#include "Koopas.h"
#include "Platform.h"
#include "Mario.h"
CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_DIE);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	vy += MARIO_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	// turn off collision when die 
	//if (state != KOOPAS_STATE_DIE)
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
		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CPlatform*>(e->obj)) 
			{
				CPlatform* plat = dynamic_cast<CPlatform*>(e->obj);
				if (plat->getType() == 3) {
					vx = -vx;
				}
				if (plat->getType() == 2) {
					vy = 0;
				}
			}
			/*else if (dynamic_cast<CMario*>(e->obj))
			{
				if (e->ny > 0) {
					SetState(KOOPAS_STATE_DIE);
				}

			}*/
		}

		// block every none-special object 
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny != 0)
			vy = 0;
	}


}

void CKoopas::Render()
{
	int ani = KOOPAS_STATE_DIE;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else {
		if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
		else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;
	}
		

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		//vx = KOOPAS_WALKING_SPEED;
		nx = 1;
	case KOOPAS_STATE_WALKING_LEFT:
		nx = -1;
	}

}

//void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
//{
//	left = x;
//	top = y;
//	bottom = y + KOOPAS_BBOX_HEIGHT;
//	if (state != KOOPAS_STATE_DIE) {
//		right = x + KOOPAS_BBOX_WIDTH;
//	}
//	else
//		right = x + KOOPAS_BBOX_HEIGHT_DIE;
//}