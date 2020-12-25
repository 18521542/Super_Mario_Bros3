#include "Stack.h"

Stack::Stack(int typ) {
	this->type = typ;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_FOR_STACK));
}

void Stack::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//Do nothing
}

void Stack::Render() {
	int ani = 0;
	if (type == TYPE_BLACK)
	{
		ani = ANI_POS_FOR_BLACK;
	}
	else
		ani = ANI_POS_FOR_WHITE;
	this->animation_set->at(ani)->Render(x, y);
}