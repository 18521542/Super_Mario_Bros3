#pragma once
#include"GameObject.h"
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