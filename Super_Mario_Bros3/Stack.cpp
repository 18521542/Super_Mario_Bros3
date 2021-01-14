#include "Stack.h"
#include "Mario.h"
#include "PlayScene.h"

Stack::Stack(int typ,float start, float end) {
	this->StartSpeed = start;
	this->EndSpeed = end;
	this->type = typ;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_FOR_STACK));
}

void Stack::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//Do nothing
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	DebugOut(L"\n vx = %f", abs(mario->vx));
	if (mario->IsRunning()) {
		if (abs(mario->vx) > StartSpeed && abs(mario->vx) < EndSpeed) {
			type = TYPE_WHITE;
		}
	}
	
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