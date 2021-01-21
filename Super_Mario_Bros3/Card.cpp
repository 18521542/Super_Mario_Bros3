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

#define DISTANCE_HEIGHT	80
#define DISTANCE_WIDHT	90

#define CARD_DISTANCE_HEIGHT	70
#define CARD_DISTANCE_WIDTH		50

#define TIME_RANDOM	150
#define MOVING_SPEED_CARD	0.07f
#define TIME_FOR_MOVING		1000
#define TIME_FOR_MOVING_LIMIT		2000
void Card::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CGameObject::Update(dt, coObjects);
	y += dy;

	if (isAppear) 
	{
		if (GetTickCount64() - StartRandomTime > TIME_RANDOM) {
			if(isAllowToRandom)
				id = this->RandomID();
			StartRandomTime = GetTickCount64();
		}
	}
	else 
	{
		x = StartX - DISTANCE_WIDHT;
		y = StartY - DISTANCE_HEIGHT;
		this->hud_card->SetPosition(this->StartX + CARD_DISTANCE_WIDTH, this->StartY - CARD_DISTANCE_HEIGHT);
		this->hud_card->SetID(id);
		if(hud_card->IsAllowToAppear())
			hud_card->StartAppear();
		hud_card->Update(dt, coObjects);
		return;
	}

	if (isMoving) {
		vy = -MOVING_SPEED_CARD;
		isEatenByMario = true;
	}
	else {
		vy = 0;
	}
	//DebugOut(L"\n Is moving %d", isMoving);

	if (GetTickCount64() - StartMovingTime > TIME_FOR_MOVING && GetTickCount64()-StartMovingTime < TIME_FOR_MOVING_LIMIT) {
		isMoving = false;
		isAppear = false;
		StartMovingTime = 0;
	}
}

int Card::RandomID() {
	return 1 + rand() % (3);
}

