#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Grid.h"

#define SCENE_SECTION_MAP	900
#define SCENE_SECTION_GRID	1000

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}




void CPlayScene::_ParseSection_Tilemap(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 8) return;

	wstring Path_Of_Tileset_Pictures = ToWSTR(tokens[0]);
	wstring Path_Of_Data_File = ToWSTR(tokens[1]);

	int TilesetColumn = atoi(tokens[2].c_str());
	int TilesetRow = atoi(tokens[3].c_str());

	int MapColumn= atoi(tokens[4].c_str());
	int Maprow = atoi(tokens[5].c_str());

	int TileWidth = atoi(tokens[6].c_str());
	int TileHeight = atoi(tokens[7].c_str());

	CTileMap::GetInstance()->LoadDetails(TilesetColumn, TilesetRow, MapColumn, Maprow, TileWidth, TileHeight);
	CTileMap::GetInstance()->LoadTileset(Path_Of_Tileset_Pictures.c_str());
	CTileMap::GetInstance()->LoadDataFromTileset(Path_Of_Data_File.c_str());
	CTileMap::GetInstance()->LoadTile();
}

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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
	if (s == NULL)
	{
		DebugOut(L"[ERROR] ani-set ID %d not found!\n", ani_set_id);
		return;
	}
	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());  //left
	float y = (float)atof(tokens[2].c_str());	//top

	int ani_set_id = atoi(tokens[3].c_str());
	

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: 
	{
		int type = atoi(tokens[4].c_str());
		obj = new CGoomba(type);
		break;
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: 
	{
		int type = atoi(tokens[4].c_str());
		obj = new CKoopas(type);
		obj->StartY = y;
		break;
	}
	case OBJECT_TYPE_BACKGROUND: 
	{
		int type = atoi(tokens[4].c_str());
		obj = new CBackground(type); 
		break; 
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r =(float) atof(tokens[4].c_str());
		float b =(float) atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_PLATFORM:
	{
		//DebugOut(L"aaaa");
		float r = (float) atof(tokens[4].c_str());
		float b = (float)atof(tokens[5].c_str());
		int type = atoi(tokens[6].c_str());
		obj = new CPlatform(x, y, r, b, type);

	}
	break;
	case OBJECT_TYPE_COIN:
	{
		int state = atoi(tokens[4].c_str());
		obj = new CCoin(state);
	}
	break;
	case OBJECT_TYPE_FIREBALL:
		obj = new CFireBall();
		fireballs.push_back((CFireBall*)obj);
		//x,y
		break;
	case OBJECT_TYPE_TAIL:
		obj = new CTail();
		TailOfMario = (CTail*)(obj);
		break;
	case OBJECT_TYPE_BREAKABLEBRICK:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CBreakableBrick(type);
		break;
	}
	case OBJECT_TYPE_QUESTIONBRICK:
		obj = new CQuestionBrick(y,x);
		break;
	case OBJECT_TYPE_LEAF_MUSHROOM:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CLeaf_Mushroom(type);
		break;
	}
	case OBJECT_TYPE_VENUS_RED:
	{
		int type = atoi(tokens[4].c_str());
		obj = new CVenus(type);
		break;
	}
	case OBJECT_TYPE_SWITCH_BLOCK:
	{
		int state = atoi(tokens[4].c_str());
		obj = new CSwitchBlock(state);
		break;
	}
	case OBJECT_TYPE_CARD:
	{
		obj = new Card();
		obj->StartX = x;
		obj->StartY = y;
		break;
	}
	case OBJECT_TYPE_MOVING_BRICK:
	{
		obj = new MovingBrick();
		break;
	}
	case OBJECT_TYPE_BOOMERANG_BROTHER:
	{
		obj = new BoomerangBrother();
		break;
	}
	case OBJECT_TYPE_MOVING_EDGE:
	{
		float stopDes = (float)atof(tokens[4].c_str());
		obj = new MovingEdge(stopDes);
		movingEdge = (MovingEdge*)obj; // moving edge start
		break;
	}
	case OBJECT_TYPE_ANCHOR:
	{
		int type = atoi(tokens[4].c_str());
		obj = new Anchor(type); // moving edge start
		if (type == ANCHOR_IN) {
			Anchor_in = (Anchor*)obj;
			DebugOut(L"\n Anchor in created");
		}
		else if (type == ANCHOR_OUT) {
			Anchor_out = (Anchor*)obj;
			DebugOut(L"\n Anchor out created");
		}
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

}

void CPlayScene::Load()
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

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
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
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
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
		case SCENE_SECTION_MAP: _ParseSection_Tilemap(line); break;
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	hud = new HUD();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::_ParseSection_GRID(string line)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	LPCWSTR path = ToLPCWSTR(tokens[0]);

	grid = new Grid(path, &objects);

	//grid->DevideObjectIntoCell()
	/*for (size_t i = 0; i < objects.size(); i++)
	{
		grid->DevideObjectIntoCell(objects[i]);
	}*/
	
}

#define MAX_Y	2900
#define Max_Y_Cam	230.0f
void CPlayScene::Update(DWORD dt)
{
	player->Update(dt, &ListObjectToCheckCollision);
	
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	cx -= (int)(game->GetScreenWidth()) / 2;
	cy -= (int)(game->GetScreenHeight() / 3);

	if (cx <= 0)
		cx = 0;
	if (cy >= Max_Y_Cam && !player->IsInSecretRoom())
		cy = Max_Y_Cam;

	DebugOut(L"\n List object size %d", objects.size());
	DebugOut(L"\n List object size %d", ListObjectToCheckCollision.size());

	//grid->Update(dt,&ListObjectToCheckCollision, cx, cy);
	if (grid != NULL) 
	{
		grid->GetListObjectsOfCell(&ListObjectToCheckCollision, cx, cy);
	}
	for (size_t i = 0; i < ListObjectToCheckCollision.size(); i++)
	{
		if (!dynamic_cast<CMario*>(ListObjectToCheckCollision[i]) && !dynamic_cast<MovingEdge*>(ListObjectToCheckCollision[i]))
		{
			ListObjectToCheckCollision[i]->Update(dt, &ListObjectToCheckCollision);
		}
	}
	TailOfMario->Update(dt, &ListObjectToCheckCollision);
	for (size_t i = 0; i < fireballs.size(); i++) {
		fireballs[i]->Update(dt, &ListObjectToCheckCollision);
	}
	//Update object

	if (movingEdge != NULL && !player->IsInSecretRoom())
	{
		movingEdge->Update(dt, &ListObjectToCheckCollision);
		for (size_t i = 0; i < fireballs.size(); i++) {
			fireballs[i]->Update(dt, &ListObjectToCheckCollision);
		}
		if (movingEdge->IsActive())
		{
			CGame::GetInstance()->SetCamPos((int)(movingEdge->x), (movingEdge->y + (game->GetScreenHeight() / 6)));
		}
		else {
			CGame::GetInstance()->SetCamPos((float)(movingEdge->GetStopDes()), (float)(movingEdge->y + (game->GetScreenHeight() / 6)));
			hud->Update(dt, &ListObjectToCheckCollision);
		}
		hud->Update(dt, &ListObjectToCheckCollision);
		return;
	}
	
	if (player->GetState() != MARIO_STATE_DIE) 
	{
		if (player->IsAtTheEnd()) {
			player->SetState(MARIO_STATE_WALKING_RIGHT);
			return;
		}
		if (!player->IsInSecretRoom()) 
		{
			CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
		}
		else
			CGame::GetInstance()->SetCamPos((int)cx, Anchor_in->y-SCREEN__HEIGHT/2);
	}
	else {
		return;
	}
	
	hud->Update(dt, &ListObjectToCheckCollision);
	
}

void CPlayScene::Render()
{
	CTileMap::GetInstance()->Render();
	
	player->Render();
	for (size_t i = 0; i < ListObjectToCheckCollision.size(); i++) {

		ListObjectToCheckCollision[i]->Render();
	}
	
	//SpecialObj->Render();
	hud->Render();
	for (size_t i = 0; i < fireballs.size(); i++) {
		fireballs[i]->Render();
	}
}

void CPlayScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	fireballs.clear();
	ListObjectToCheckCollision.clear();

	player = NULL;
	TailOfMario = NULL;
	movingEdge = NULL;
	Anchor_in = NULL;
	Anchor_out = NULL;

	CTileMap::GetInstance()->Unload();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode)
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	HUD* hud = ((CPlayScene*)scence)->GetHud();
	if (mario->IsAtTheEnd())
		return;
	if (mario->GetState() == MARIO_STATE_DIE)
		return;
	if (mario->IsEntering())
		return;
	switch (KeyCode)
	{
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_4:
		mario->SetLevel(MARIO_LEVEL_FROG);
		break;
	case DIK_5:
		mario->SetLevel(MARIO_LEVEL_HAMMER);
		break;
	case DIK_6:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_S:
		float currentVx, currentVy;
		mario->GetSpeed(currentVx, currentVy);
		if (mario->GetLevel() != MARIO_LEVEL_TAIL) {
			if (mario->IsReadyToJump() && !mario->IsFlying())
			{
				mario->SetState(MARIO_STATE_JUMP);
				if (mario->IsReadyToJumpFly())
				{
					mario->setIsJumpFlying(true);
				}
			}
		}
		else
		{
			if (mario->IsReadyToJump() && !mario->IsFlying())
			{
				mario->SetState(MARIO_STATE_JUMP);
				if (mario->IsReadyToJumpFly())
				{
					mario->setIsJumpFlying(true);
				}
				break;
			}
			if (mario->IsReadyToFly())
			{
				mario->StartFlying();
				mario->setIsReadyToFly(false);
				break;
			}
			if (mario->IsFlying()) {
				mario->SetState(MARIO_STATE_FLY);
				break;
			}
			if (mario->IsOnAir())
			{
				if (!mario->IsFlying() || !mario->IsReadyToJump()) 
				{
					//mario->SetSpeed(currentVx, MARIO_FALLING_SPEED);
					mario->vy = MARIO_FALLING_SPEED;
					mario->setIsFalling(true);
					break;
				}
			}
		}
		break;
	case DIK_R:
		mario->Reset();
		break;
	case DIK_A:
		
		mario->SetIsReadyToHold(true);
		mario->setIsReadyToRun(true);
		if (!mario->IsFlying())
			hud->StackStartUp();
		if (mario->GetLevel() == MARIO_LEVEL_TAIL) 
		{
			mario->StartUsingTail();
			break;
		}
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
		{
			mario->StartShootingFireBall();
			break;
		}
			
		break;
	case DIK_0:
		CGame::GetInstance()->SwitchScene(2);
		break;
	case DIK_DOWN:
		if(mario->IsForSecretRoom())
			mario->StartEnter();
		break;
	case DIK_UP:
		mario->SetIsReadyToExit(true);
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	HUD* hud = ((CPlayScene*)scence)->GetHud();
	if (mario->IsAtTheEnd())
		return;
	if (mario->GetState() == MARIO_STATE_DIE)
		return;
	switch (KeyCode)
	{
	case DIK_UP:
		mario->SetIsReadyToExit(false);
		break;
	case (DIK_S):
		if(mario->GetState()!=MARIO_STATE_IDLE)
			mario->SetState(MARIO_STATE_IDLE);

		mario->setIsReadyToJump(false);

		mario->setIsReadyToSit(true);

		if (mario->IsReadyToJumpFly()) {
			mario->setIsReadyToJumpFlying(false);
		}
		mario->setIsJumpFlying(false);
		if (mario->IsFalling()) {
			mario->setIsFalling(false);
			mario->SetState(MARIO_STATE_IDLE);
		}
		break;
	case (DIK_LEFT):
		if (mario->GetState() == MARIO_STATE_WALKING_LEFT)
			mario->SetState(MARIO_STATE_IDLE);
		if (mario->IsRunning())
		{
			mario->setIsRunning(false);
			mario->SetState(MARIO_STATE_IDLE);
			mario->setIsReadyToRun(false);
		}
			
		mario->setIsReadyToSit(true);
		break;
	case (DIK_RIGHT):
		if(mario->GetState()==MARIO_STATE_WALKING_RIGHT)
			mario->SetState(MARIO_STATE_IDLE);
		if (mario->IsRunning())
		{
			mario->setIsRunning(false);
			mario->SetState(MARIO_STATE_IDLE);
			mario->setIsReadyToRun(false);
		}
		//mario->setIsAllowToStop(true);
		mario->setIsReadyToSit(true);
		break;
	case (DIK_DOWN):
		mario->setIsReadyToSit(true);
		if (mario->GetState() == MARIO_STATE_SIT) 
		{
			mario->SetState(MARIO_STATE_IDLE);
			break;
		}
		break;
	case DIK_A:
		mario->SetIsReadyToHold(false);
		mario->setIsReadyToRun(false);
		mario->setIsRunning(false);
		hud->StackStartDown();
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
		{
			mario->SetState(MARIO_STATE_IDLE);
			break;
		}
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	HUD* hud = ((CPlayScene*)scence)->GetHud();

	if (mario->IsAtTheEnd())
		return;
	// disable control key when Mario die 
	if (mario->IsEntering())
		return;
	if (mario->GetState() == MARIO_STATE_DIE)
		return;
	else {
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (game->IsKeyDown(DIK_S))
			{
				mario->nx = 1;
				mario->vx = 0.09f;
			}
			else if (mario->IsReadyToRun())
			{
				mario->setIsRunning(true);
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
				mario->setIsReadyToSit(false);
			}
			else {
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
				mario->setIsReadyToSit(false);
			}
		}
		else if (game->IsKeyDown(DIK_LEFT)) {
			if (game->IsKeyDown(DIK_S))
			{
				mario->nx = -1;
				mario->vx = -0.09f;
			}
			else if (mario->IsReadyToRun()) {
				mario->setIsRunning(true);
				mario->SetState(MARIO_STATE_WALKING_LEFT);
				mario->setIsReadyToSit(false);
			}
			else {
				mario->SetState(MARIO_STATE_WALKING_LEFT);
				mario->setIsReadyToSit(false);
			}
		}
		else if (game->IsKeyDown(DIK_DOWN)) {
			if (mario->GetLevel() != MARIO_LEVEL_SMALL)
				if (mario->IsReadyToSit()&&!mario->IsForSecretRoom())
					mario->SetState(MARIO_STATE_SIT);
		}
	}
}