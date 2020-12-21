#pragma once
#include "GameObject.h"
#define ANI_SET_PIECE		8
#define ANI_PIECE			3

class Piece : public CGameObject
{
public:
	Piece();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
};


class PiecesANI
{
	ULONGLONG StartAppearTime = 0;
	bool isAppear = false;

	vector<Piece*> pieces;

public:
	PiecesANI(float x, float y);
	~PiecesANI();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	void Render();

	void StartAppear() { StartAppearTime = GetTickCount64(); isAppear = true; }
};