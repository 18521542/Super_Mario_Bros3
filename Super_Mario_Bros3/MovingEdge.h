#pragma once
#include "GameObject.h"
#define SCREEN__WIDTH 330
#define SCREEN__HEIGHT 290

#define EDGE_TYPE_START	1
#define EDGE_TYPE_END	2

class MovingEdge : public CGameObject {
	int type;
protected:
	float  StopDes;
	bool isActive = true;
public :
	int GetType() { return this->type; }
	void SetType(int typ) { this->type = typ; }
	bool IsActive() { return this->isActive; }
	MovingEdge(float stopDes);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};