#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN	-1
#define GAME_FILE_SECTION_SETTINGS	1
#define GAME_FILE_SECTION_SCENES	2
#define GAME_FILE_SECTION_HUD		3

#define PLAY_SCENE					1
#define WORLDMAP_SCENE				2
#define INTRO_SCENE					3

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Scene.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;
	ULONGLONG TimeToSwitchScene;
	bool isSwitch = false;


	//defaut HUD
	int World;
	int MarioSpeed = 0;
	float Time = 300.0f;
	int Score =0;
	int Life;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
	void _ParseSection_HUD(string line);
public:
	int GetWorld() { return World; }
	int GetMarioSpeed(){ return MarioSpeed; }
	float GetTime() { return Time; }
	int GetScore() { return Score; }
	int GetLife() { return Life; }

	void SetWorld(int value) { World = value; }
	void SetMarioSpeed(int value) { MarioSpeed = value; }
	void SetTime(float value) { Time = value; }

	void ScoreUp(int value) { Score += value; }
	void ScoreDown(int value) { Score -= value; }

	void SetLife(int value) { Life = value; }

	void StartSwitchScene() {
		TimeToSwitchScene = GetTickCount64();
		isSwitch = true;
	}
	bool IsSwitch() { return isSwitch; }
	void SetIsSwitch(bool switched) { isSwitch = switched; }
	ULONGLONG GetTimeStartSwitch() { return TimeToSwitchScene; }
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }
	void GetPositionCam(float& x, float& y) { x = cam_x; y = cam_y; }
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	static CGame* GetInstance();

	~CGame();
};


