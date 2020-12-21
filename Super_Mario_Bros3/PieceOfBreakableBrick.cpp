#include "PieceOfBreakableBrick.h"

Piece::Piece() {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_PIECE));
}

void Piece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	vy += 0.0005f * dt;

	x += dx;
	y += dy;
}

void Piece::Render() {
	animation_set->at(ANI_PIECE)->Render(x, y);
}

PiecesANI::PiecesANI(float x, float y) {

	Piece* piece;

	for (int i = 0; i < 4; i++) {
		piece = new Piece();
		piece->SetPosition(x, y);
		pieces.push_back(piece);
	}

	pieces.at(0)->SetSpeed(-0.02f, -0.15f);
	pieces.at(1)->SetSpeed(-0.02f, -0.1f);
	pieces.at(2)->SetSpeed(0.02f, -0.15f);
	pieces.at(3)->SetSpeed(0.02f, -0.1f);
}

PiecesANI::~PiecesANI()
{
	for (int i = 0; i < 4; i++)
		delete(pieces[i]);
	pieces.clear();
}

void PiecesANI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject) 
{
	if (isAppear) 
	{
		for (Piece* piece : pieces)
		{
			piece->Update(dt, coObject);
		}
	}
	if (GetTickCount64() - StartAppearTime > 1500) 
	{
		isAppear = false;
		StartAppearTime = 0;
	}
}

void PiecesANI::Render() 
{
	if (isAppear) 
	{
		for (Piece* piece : pieces) {
			piece->Render();
		}
	}
	
}
