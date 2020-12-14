#pragma once
#include "GameObject.h"
#define BACKGROUND_BBOX_WIDTH  2821
#define BACKGROUND_BBOX_HEIGHT 439

#define BIG_MAP					1
#define GREEN_PIPE				2

class CBackground : public CGameObject
{
	int type;
public:
	CBackground(int type);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

