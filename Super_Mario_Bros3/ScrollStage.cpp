#include "ScrollStage.h"

ScrollStage::ScrollStage() {
	vy = -0.04f;
	/*this->SetAnimationSet()*/
}
void ScrollStage::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	y += dy;
}
void ScrollStage::Render() {
	this->animation_set->at(0)->Render(x, y);
}