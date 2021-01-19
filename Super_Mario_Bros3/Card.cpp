#include "Card.h"
#include "Utils.h"
Card::Card() {
	id = STAR;
	isAppear = true;
	StartRandomTime = GetTickCount64();
	hud_card = new HUD_Card(PLANT);
}

void Card::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
}
void Card::Render() {
	int ani = -1;
	if (id == STAR) {
		if (isEatenByMario) {
			ani = ANI_POS_STAR_IS_EATEN;
		}
		else ani = ANI_POS_STAR_IS_NOT_EATEN;
	}
	else if (id == MUSHROOM_CARD) {
		if (isEatenByMario) {
			ani = ANI_POS_MUSHROOM_IS_EATEN;
		}
		else ani = ANI_POS_MUSHROOM_IS_NOT_EATEN;
	}
	else if (id == PLANT) {
		if (isEatenByMario) {
			ani = ANI_POS_PLANT_IS_EATEN;
		}
		else ani = ANI_POS_PLANT_IS_NOT_EATEN;
	}

	
	if (isAppear) {
		animation_set->at(ani)->Render(x, y);
		//DebugOut(L"\nAni render");
	}
	else animation_set->at(ANI_POS_LETTER_END)->Render(x, y);

	hud_card->Render();
}

void Card::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	
	if (isAppear) 
	{
		if (GetTickCount64() - StartRandomTime > 150) {
			if(isAllowToRandom)
				id = this->RandomID();
			StartRandomTime = GetTickCount64();
		}
	}
	else 
	{
		x = 2600;
		hud_card->SetPosition(x + 140, y +15);
		hud_card->SetID(id);
		if(hud_card->IsAllowToAppear())
			hud_card->StartAppear();
		hud_card->Update(dt, coObjects);
	}

	if (isMoving) {
		vy = -0.07f;
		isEatenByMario = true;
	}
	else {
		vy = 0;
	}
	//DebugOut(L"\n Is moving %d", isMoving);

	y += dy;

	if (GetTickCount64() - StartMovingTime > 1000 && GetTickCount64()-StartMovingTime <2000) {
		isMoving = false;
		isAppear = false;
		StartMovingTime = 0;
	}
}

int Card::RandomID() {
	return 1 + rand() % (3);
}

