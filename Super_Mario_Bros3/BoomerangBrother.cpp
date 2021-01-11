#include "BoomerangBrother.h"
#define BOM_BRO_STATE_WALKING	1
#define BOM_BRO_STATE_FIRING	2


BoomerangBrother::BoomerangBrother() {
	FirstWeapon = new Boomerang();
	SecondWeapon = new Boomerang();
}
void BoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom) {

}
void BoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	FirstWeapon->Update(dt,coObjects);
	SecondWeapon->Update(dt,coObjects);

	if (!isInitedWeapon) {
		FirstWeapon->SetPosition(x, y);
		SecondWeapon->SetPosition(x, y);
		isInitedWeapon = true;
	}
}
void BoomerangBrother::Render() 
{
	animation_set->at(1)->Render(x, y);
	FirstWeapon->Render();
	SecondWeapon->Render();
}

