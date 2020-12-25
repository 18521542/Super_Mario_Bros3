#include "Power.h"

Power::Power(int typ) {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_FOR_POWER));
	this->type = typ;
}

void Power::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//Do nothing
}

void Power::Render() {
	int ani = 0;
	if (type == TYPE_NORMAL)
	{
		ani = ANI_POS_WHEN_NORMAL;
	}
	else
		ani = ANI_POS_WHEN_ACTIVATED;
	this->animation_set->at(ani)->Render(x, y);
}