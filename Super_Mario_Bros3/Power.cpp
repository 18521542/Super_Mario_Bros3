#include "Power.h"
#include "Mario.h"
#include "PlayScene.h"
Power::Power(int typ) {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_FOR_POWER));
	this->type = typ;
}

void Power::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (CGame::GetInstance()->GetCurrentScene()->GetID() == WORLDMAP_SCENE) {
		return;
	}
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsRunning()||mario->IsFlying()) {
		
		if(mario->IsFlying())
			this->type = TYPE_ACTIVATED;
		if (mario->IsRunning()) {
			if (mario->vx == MARIO_RUN_SPEED_MAX)
				this->type = TYPE_ACTIVATED;
		}
	}
	else
	{
		this->type = TYPE_NORMAL;
	}
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