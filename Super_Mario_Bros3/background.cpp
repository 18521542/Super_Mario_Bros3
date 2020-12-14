#include "Background.h"

CBackground::CBackground(int type) {
	this->type = type;
}
void CBackground::Render()
{
	if(type==BIG_MAP)
		animation_set->at(0)->Render(x, y);
	else if (type == GREEN_PIPE) {
		animation_set->at(1)->Render(x, y);
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