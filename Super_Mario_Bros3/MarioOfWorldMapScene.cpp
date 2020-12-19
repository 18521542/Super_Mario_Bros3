#include "MarioOfWorldMapScene.h"
#include "CheckPoint.h"
#include "Utils.h"
MarioWorldMap::MarioWorldMap() {
	right = true;
	bottom = false;
	top = false;
	left = true;
}
void MarioWorldMap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	for (size_t i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		float pLeft, pTop, pRight, pBottom;
		obj->GetBoundingBox(pLeft, pTop, pRight, pBottom);
		if (dynamic_cast<CheckPoint*>(obj)) 
		{
			CheckPoint* cp = dynamic_cast<CheckPoint*>(obj);
			if (this->CheckBB(pLeft, pTop, pRight, pBottom) && cp->IsAbleToStop()) 
			{
				this->vx = 0;
				this->vy = 0;
				x += dx;
				y += dy;
				x = cp->x;
				y = cp->y;
				int l, t, r, b;
				cp->GetCheckPoint(l, t, r, b);
				this->SetDirection(l, t, r, b);
				cp->SetIsAbleToStop(false);
				cp->SetIsStop(true);
				isMoving = false;
			}
		}
	}
}

void MarioWorldMap::SetDirection(int l, int t, int r, int b) {
	SetTrueFalseByValue(this->left,		l);
	SetTrueFalseByValue(this->top,		t);
	SetTrueFalseByValue(this->right,	r);
	SetTrueFalseByValue(this->bottom,	b);
}
void MarioWorldMap::Render() 
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void MarioWorldMap::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x+MARIO_WM_WIDTH;
	b = y+ MARIO_WM_HEIGHT;
}