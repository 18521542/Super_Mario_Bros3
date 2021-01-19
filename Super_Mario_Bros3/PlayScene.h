#pragma once
#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_BACKGROUND			4
#define OBJECT_TYPE_PLATFORM			5
#define OBJECT_TYPE_COIN				6
#define OBJECT_TYPE_PORTAL				50
#define OBJECT_TYPE_FIREBALL			7
#define OBJECT_TYPE_TAIL				8
#define OBJECT_TYPE_BREAKABLEBRICK		9
#define OBJECT_TYPE_QUESTIONBRICK		10
#define OBJECT_TYPE_LEAF_MUSHROOM		11
#define OBJECT_TYPE_VENUS_RED			12
#define OBJECT_TYPE_SWITCH_BLOCK		13
#define OBJECT_TYPE_CARD				14
#define OBJECT_TYPE_MOVING_BRICK		129
#define OBJECT_TYPE_BOOMERANG_BROTHER	130
#define OBJECT_TYPE_MOVING_EDGE			131
#define OBJECT_TYPE_ANCHOR				132

#define MAX_SCENE_LINE 1024


#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"

#include "Object.h"

#include "Tilemap.h"
#include "Grid.h"
#include "HUD.h"

class CPlayScene : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 
	CTail* TailOfMario;
	vector<CFireBall*> fireballs;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> ListObjectToCheckCollision;
	//vector<LPGAMEOBJECT> SpecialObjects;
	HUD* hud;
	Grid* grid;

	MovingEdge* movingEdge;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_Tilemap(string line);
	void _ParseSection_GRID(string line);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	vector<LPGAMEOBJECT> GetListObject() { return this->objects; }

	CMario* GetPlayer() { return player; }
	HUD* GetHud() { return hud; }
	MovingEdge* GetMovingEdge() { return movingEdge; }
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

