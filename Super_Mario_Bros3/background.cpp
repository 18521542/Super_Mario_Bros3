#include "Background.h"
#include "Utils.h"
#define ANI_SET_FOR_BACKGROUND_OF_HUD	999999
CBackground::CBackground(int type) 
{
	this->type = type;
}

CBackground::CBackground() {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_FOR_BACKGROUND_OF_HUD));
	type = BIG_MAP;
}
void CBackground::Render()
{
	if(type==BIG_MAP)
		animation_set->at(ANI_POS_BIG)->Render(x, y);
	else if (type == GREEN_PIPE) {
		animation_set->at(ANI_POS_GREEN_PIPE)->Render(x, y);
	}
	else if (type == GRAY_PIPE) {
		animation_set->at(ANI_POS_GRAY_PIPE)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = 0;
	t = 0;
	r = 0;
	b = 0;
}