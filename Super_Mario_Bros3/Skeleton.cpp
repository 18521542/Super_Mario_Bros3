#include"Skeleton.h"

Skeleton::Skeleton(int typ) {
	this->type = typ;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_FOR_SKELETON));
}

void Skeleton::Render() {
	int ani = 0;
	if (type == MAIN_SKELETON)
	{
		ani = MAIN_SKELETON_POS;
	}
	else
		ani = CARD_SKELETON_POS;
	this->animation_set->at(ani)->Render(x, y);
}