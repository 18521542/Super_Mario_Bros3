#include "PieceOfBreakableBrick.h"

Piece::Piece() {
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANI_SET_PIECE));
}

#define ACCELERATION_OF_PIECE	0.0005f
void Piece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	vy += ACCELERATION_OF_PIECE * dt;

	x += dx;
	y += dy;
}

void Piece::Render() {
	animation_set->at(ANI_PIECE)->Render(x, y);
}

#define UP		-0.15f
#define DOWN	-0.1f
#define RIGHT	0.02f
#define LEFT	-0.02f
PiecesANI::PiecesANI(float x, float y) {

	Piece* piece;

	for (int i = 0; i < 4; i++) {
		piece = new Piece();
		piece->SetPosition(x, y);
		pieces.push_back(piece);
	}

	pieces.at(0)->SetSpeed(LEFT, UP);
	pieces.at(1)->SetSpeed(LEFT, DOWN);
	pieces.at(2)->SetSpeed(RIGHT, UP);
	pieces.at(3)->SetSpeed(RIGHT, DOWN);
}

PiecesANI::~PiecesANI()
{
	for (int i = 0; i < 4; i++)
		delete(pieces[i]);
	pieces.clear();
}

#define APPEAR_TIME	1500
void PiecesANI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject) 
{
	if (isAppear) 
	{
		for (Piece* piece : pieces)
		{
			piece->Update(dt, coObject);
		}
	}
	if (GetTickCount64() - StartAppearTime > APPEAR_TIME)
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
