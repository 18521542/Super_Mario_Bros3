#include "Background.h"

CBackground::CBackground(int type) 
{
	this->type = type;
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