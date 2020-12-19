#pragma once
#include "GameObject.h"
class CheckPoint : public CGameObject {
	bool left;
	bool top;
	bool right;
	bool bottom;

	int width;
	int height;

	bool isAbleToStop = true;
	bool isStopped = false;
	ULONGLONG ResetIsAbleToStop;
public :
	CheckPoint(int l, int t, int r, int b, int _width, int _height);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetCheckPoint(bool& postion, int value);
	void GetCheckPoint(int& l, int& t, int& r, int& b) { l = this->left; t = this->top; r = this->right; b = this->bottom; };

	bool IsAbleToStop() { return this->isAbleToStop; }

	void StartReset() { ResetIsAbleToStop = GetTickCount64(); isStopped = false; isAbleToStop = true; }

	void SetIsAbleToStop(int value) { this->isAbleToStop = value; }
	void SetIsStop(int value) { this->isStopped = value; }
};