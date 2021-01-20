#pragma once
#include"Object.h"
#include "PieceOfBreakableBrick.h"
#define BREAKABLE_BRICK_STATE_DISAPPEAR 1

#define NORMAL							1
#define SPECIAL							2
#define SPECIAL_1_4						3

#define COIN							6
#define SHINING							2
#define	WITHOUT_SHINING					3

#define ANI_COIN						1
#define ANI_WITHOUT_SHINING				2
#define ANI_SHINING						0

class CBreakableBrick : public CGameObject 
{
	float startX, startY;
	int type;

	PiecesANI* pieceANI;
	bool isInitPieceANI = false;

	ULONGLONG CoinAppearTime;
	bool isNotAllowAniBreakAppear = false;

	bool isActivated = false;
	bool isReturnedCoin = false;

	bool isMoving = false;
	int Life = 3;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	bool isBeingBroken = false;
	DWORD StartBroken;
	ULONGLONG TimeStartMove;
public:
	CBreakableBrick(int typ);
	int GetType() { return this->type; }
	void SetCoinAppear() 
	{
		CoinAppearTime = GetTickCount64();
		state = COIN;
	}

	void DecreaseLife() {
		Life--;
	}
	int life() { return Life; }

	void SetIsReturned(bool activa) {
		this->isReturnedCoin = activa;
	}
	void SetStartPos(float x, float y) { this->startX = x; this->startY = y; }
	void StartMoving() { vy = -0.1f; isMoving = true; TimeStartMove = GetTickCount64(); }
	bool IsReturned() { return this->isReturnedCoin; }
	bool IsMoving() { return this->isMoving; }
};