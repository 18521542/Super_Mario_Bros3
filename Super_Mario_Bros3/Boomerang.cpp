#include "Boomerang.h"

Boomerang::Boomerang() 
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(BOM_BRO_ANI_SET));
}

void Boomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom) {

}
void Boomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	vx = 0.01f;
	vy = 0;
	x += dx;
	y += dy;
}
void Boomerang::Render() {
	animation_set->at(2)->Render(x, y);
}
