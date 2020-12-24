#pragma once
#include "Scene.h"
#include "Game.h"
#include "GameObject.h"
#include "Object.h"
#include "Textures.h"
#include "MarioOfWorldMapScene.h"
#include "Help.h"
#include "ArrowToChoose.h"

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BACKGROUND			4

#define MAX_SCENE_LINE 1024

class IntroScene : public CScene
{
	vector<LPGAMEOBJECT> objects;
	Arrow* arrow;
protected:
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	IntroScene(int id, LPCWSTR filePath);
	Arrow* GetPlayer() { return arrow; }
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
};

class IntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);

	IntroSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
}; 
