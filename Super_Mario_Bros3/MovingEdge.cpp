#include "MovingEdge.h"
#include "PlayScene.h"
#define SPEED		0.03f



void MovingEdge::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x+3;
	bottom = y+SCREEN__HEIGHT;
}
void MovingEdge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGameObject::Update(dt, coObjects);
	x += dx;

	if (mario->x < this->x )
		mario->x = this->x;
	if (this->x >= this->StopDes) {
		vx = 0;
		isActive = false;
		return;
	}
}
void MovingEdge::Render() {
	RenderBoundingBox();
}

MovingEdge::MovingEdge(float stopDes) {
	vx = SPEED;
	this->StopDes = stopDes;
}