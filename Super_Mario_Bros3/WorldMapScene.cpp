#include "WorldMapScene.h"
#include <iostream>
#include <fstream>
#include "Textures.h"
#include "MarioOfWorldMapScene.h"
#include "CheckPoint.h"
#include "TurtleOFWorldMap.h"
#include "HUD.h"

#define INVALID_SCENE	-1

WorldMapScene::WorldMapScene(int id, LPCWSTR filePath) :CScene(id, filePath)
{
	key_handler = new WorldMapScenceKeyHandler(this);
}


void WorldMapScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
	DebugOut(L"[INFO] Texture load ok\n");
}
void WorldMapScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void WorldMapScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (size_t i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}
	if (ani == NULL)
	{
		DebugOut(L"[ERROR] ani ID %d not found!\n", ani_id);
		return;
	}
	CAnimations::GetInstance()->Add(ani_id, ani);
}
void WorldMapScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (size_t i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}
void WorldMapScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());  //left
	float y = atof(tokens[2].c_str());	//top

	int ani_set_id = atoi(tokens[3].c_str());


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_BACKGROUND:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CBackground(type);
		break;
	}
	case 100:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new MarioWorldMap();
		player = (MarioWorldMap*)obj;
		break;
	}
	case 200:
	{
		int left = atoi(tokens[4].c_str());
		int top = atoi(tokens[5].c_str());
		int right = atoi(tokens[6].c_str());
		int bottom = atoi(tokens[7].c_str());
		int width = atoi(tokens[8].c_str());
		int height = atoi(tokens[9].c_str());
		int scene_id = atoi(tokens[10].c_str());
		obj = new CheckPoint(left, top, right, bottom, width, height, scene_id);
		break;
	}
	case 300:
	{
		obj = new Turle();
		break;
	}
	case 400:
	{
		obj = new Help();
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
	hud = new HUD();
}


void WorldMapScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; 
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}
void WorldMapScene::Update(DWORD dt) 
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	hud->Update(dt, &coObjects);
	if (player == NULL) return;
	
}
void WorldMapScene::Render()
{
	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->Render();
	hud->Render();
}
void WorldMapScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	delete hud;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


void WorldMapScenceKeyHandler::KeyState(BYTE* states) {

}
void WorldMapScenceKeyHandler::OnKeyDown(int KeyCode) {
	MarioWorldMap* mario = ((WorldMapScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_LEFT:
		if (mario->GetLeft() && !mario->IsMoving()) {
			mario->vx = -MARIO_SPEED;
			mario->SetIsMoving(true);
		}
			
		break;
	case DIK_RIGHT:
		if (mario->GetRight() && !mario->IsMoving()) {
			mario->vx = MARIO_SPEED;
			mario->SetIsMoving(true);
		}
			
		break;
	case DIK_UP:
		if (mario->GetTop() && !mario->IsMoving()) {
			mario->vy = -MARIO_SPEED;
			mario->SetIsMoving(true);
		}
			
		break;
	case DIK_DOWN:
		if (mario->GetBottom() && !mario->IsMoving()) {
			mario->vy = MARIO_SPEED;
			mario->SetIsMoving(true);
		}		
		break;
	case DIK_W:
		if (mario->SceneID() != INVALID_SCENE && !mario->IsMoving()) {
			CGame::GetInstance()->SwitchScene(mario->SceneID() );
		}
		break;
	}
}
void WorldMapScenceKeyHandler::OnKeyUp(int KeyCode) {

}

