#include "Number.h"

Number::Number(int val) 
{
	this->value = val;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_NUMBER));
}

void Number::Render() {
	int ani = value;
	this->animation_set->at(value)->Render(x, y);
}

void Number::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//do nothing
}