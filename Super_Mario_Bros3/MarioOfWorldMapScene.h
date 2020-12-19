#pragma once
#include "GameObject.h"
#define MARIO_SPEED		0.13f;
#define MARIO_WM_WIDTH	1
#define MARIO_WM_HEIGHT	1
class MarioWorldMap : public CGameObject {
	bool left;
	bool top;
	bool right;
	bool bottom;

	bool isMoving = false;

	int Scene_ID_Of_Standing_CheckPoint;
public :
	MarioWorldMap();
	void SetDirection(int l, int t, int r, int b);

	bool GetLeft() { return this->left; }
	bool GetTop() {return this->top; }
	bool GetRight() { return this->right; }
	bool GetBottom() { return this->bottom; }

	void SetTrueFalseByValue(bool& dir,int value) { if (value == TRUE) dir = true; else dir = false; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetIsMoving(bool value) { this->isMoving = value; }

	int SceneID() { return this->Scene_ID_Of_Standing_CheckPoint; }
	
	bool IsMoving() { return this->isMoving; }
};