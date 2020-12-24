#include "ArrowToChoose.h"

Arrow::Arrow() {
	curChoice = 1;
	isAllowToSwitch = true;
}

void Arrow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//do nothing
}

void Arrow::Render() {
	int ani = -1;
	animation_set->at(0)->Render(x, y);
}
