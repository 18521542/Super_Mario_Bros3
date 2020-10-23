#pragma once
#include"GameObject.h"
#define PLATFORM_TYPE_TWO	2
#define PLATFORM_TYPE_ONE	1
#define PLATFORM_TYPE_THREE	3
class CPlatform : public CGameObject
{
	int width;
	int height;
	int type;
public:
	CPlatform(float l, float t, float r, float b, int typ);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int getType() { return type; };
};