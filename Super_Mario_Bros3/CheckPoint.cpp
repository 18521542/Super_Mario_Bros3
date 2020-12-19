#include "CheckPoint.h"
#include "MarioOfWorldMapScene.h"
#include "WorldMapScene.h"
#include "Utils.h"
#define TRUE	1
#define FALSE	0
CheckPoint::CheckPoint(int l, int t, int r, int b, int _width, int _height, int scene_id) {
	SetCheckPoint(left, l);
	SetCheckPoint(top, t);
	SetCheckPoint(right, r);
	SetCheckPoint(bottom, b);
	this->width = _width;
	this->height = _height;
	this->scene_id = scene_id;
}

void CheckPoint::SetCheckPoint(bool& postion, int value) {
	if (value == TRUE) {
		postion = true;
	}
	else
		postion = false;
}

void CheckPoint::Render() 
{
	RenderBoundingBox();
}

void CheckPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void CheckPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	MarioWorldMap* mario = ((WorldMapScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float left, top, right, bottom;
	mario->GetBoundingBox(left, top, right, bottom);
	if (!this->CheckBB(left, top, right, bottom)) 
	{
		StartReset();
	}
}