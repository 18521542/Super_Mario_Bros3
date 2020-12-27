#include "HUD_Card.h"
#define STAR			1
#define PLANT			2
#define MUSHROOM_CARD	3

#include "Utils.h"
HUD_Card::HUD_Card(int id) {
	this->id = id;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(123321));
	isAppear = false;
}

void HUD_Card::Render(){
	int ani = -1;
	if (id == PLANT) {
		ani = ANI_PLANT;
	}
	else if (id == MUSHROOM_CARD) {
		ani = ANI_MUSHROOM;
	}
	else if (id == STAR) {
		ani = ANI_STAR;
	}
	if(isAppear)
		animation_set->at(ani)->Render(x,y);
}

void HUD_Card::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	if (GetTickCount64() - StartAppearTime > 1000 /*&& GetTickCount64() - StartAppearTime<2000*/)
	{
		if (!isAppear)
			isAppear = true;
	}
}