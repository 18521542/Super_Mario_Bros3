#pragma once
#include "GameObject.h"
#define BACKGROUND_BBOX_WIDTH  2821
#define BACKGROUND_BBOX_HEIGHT 439

#define BIG_MAP					1
#define GREEN_PIPE				2
#define GRAY_PIPE				3

#define ANI_POS_BIG				0
#define ANI_POS_GREEN_PIPE		1
#define ANI_POS_GRAY_PIPE		2

class CBackground : public CGameObject
{
	int type;
public:
	CBackground();
	CBackground(int type);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

