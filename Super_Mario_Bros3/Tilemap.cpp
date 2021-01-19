#include "TileMap.h"
#include "Textures.h"
#include "Mario.h"
#include <iostream>
#include <fstream>
#include "Tilemap.h"
#define ID_OF_TILESET_TEXTURES	1000100


void CTileMap::LoadTileset(LPCWSTR Path_Of_Tileset_File) 
{
	CTextures* textures = CTextures::GetInstance();
	textures->Add(ID_OF_TILESET_TEXTURES, Path_Of_Tileset_File, D3DCOLOR_XRGB(255, 0, 255));
	this->TilesSetFile = textures->Get(ID_OF_TILESET_TEXTURES);
}

void CTileMap::LoadDataFromTileset(LPCWSTR Path_Of_Data_File) 
{

	ifstream f;

	f.open(Path_Of_Data_File);
	Data = new int* [RowOfMap];

	for (int i = 0; i < RowOfMap; i++) {
		Data[i] = new int[ColumnsOfMap];
		for (int j = 0; j < ColumnsOfMap; j++) {
			f >> Data[i][j];
		}
	}
	f.close();

	//for (int i = 0; i < ROWS; i++) {
	//	//Data[i] = new int[COLUMNS];
	//	DebugOut(L"\n");
	//	for (int j = 0; j < COLUMNS; j++) {
	//		DebugOut(L" Data[i][j] = %i", CTileMap::GetInstance()->Data[i][j]);
	//	}
	//}
}

void CTileMap::LoadTile() 
{
	int index = 1;
	for (int i = 0; i < RowOfTileset; i++)
	{
		for (int j = 0; j < ColumnOfTileset; j++) {

			int left = j * _TileWidth;
			int top = (i)*_TileHeight;
			int right = (j + 1) * _TileWidth;
			int bottom = (i + 1) * _TileHeight;
			LPSPRITE sprite = new CSprite(index, left, top, right, bottom, TilesSetFile);
			Tiles.push_back(sprite);
			index++;
		}
	}

	/*int value = 1;
	for (int i = 0; i < ROW_OF_TILE_SET; i++)
	{
		DebugOut(L"\n");
		for (int j = 0; j < COLUMN_OF_TILE_SET; j++)
		{
			int left = j * TILE_WIDTH;
			int top = (i) * TILE_HEIGHT;
			int right = (j+1)*TILE_WIDTH;
			int bottom = (i+1)*TILE_HEIGHT;

			DebugOut(L" %i", left);
			DebugOut(L" %i", top);
			DebugOut(L" %i", right);
			DebugOut(L" %i", bottom);
			DebugOut(L" ||");
			value++;
		}
	}*/
}

void CTileMap::LoadDetails(int TilesetColumns, int TilesetRows, int MapColumns, int MapRows, int TileWidth, int TileHeight) {

	ColumnOfTileset = TilesetColumns;
	RowOfTileset = TilesetRows;

	ColumnsOfMap = MapColumns;
	RowOfMap = MapRows;

	_TileHeight = TileHeight;
	_TileWidth = TileWidth;
}

CTileMap* CTileMap::instance = NULL;

CTileMap* CTileMap::GetInstance() {
	if (instance == NULL)
		instance = new CTileMap();
	return instance;
}

void CTileMap::Render() 
{
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	int deltaX = CGame::GetInstance()->GetScreenWidth()/ _TileWidth + 3;
	int deltaY = CGame::GetInstance()->GetScreenHeight()/ _TileHeight + 3;

	//decide which area of map will be rendered
	int startX = (int)cx/ _TileWidth;
	int finishX = (int)cx / _TileWidth + deltaX;
	int startY = (int)cy / _TileWidth;
	int finishY = (int)cy / _TileWidth + deltaY;

	if (startY < 0) {
		startY= 0;
	}

	if (finishY > RowOfMap) {
		finishY = RowOfMap;
	}

	if (finishX > ColumnsOfMap) {
		finishX = ColumnsOfMap;
	}
	/*DebugOut(L"\n======================");
	DebugOut(L"\n Start X %d", startX);
	DebugOut(L"\n finishX  %d", finishX);
	DebugOut(L"\n Start Y %d", startY);
	DebugOut(L"\n finishY %d", finishY);*/
	
	for (int i = startY; i < finishY; i++) {
		for (int j = startX; j < finishX; j++) {
			if(Data)
				CTileMap::GetInstance()->Tiles[CTileMap::GetInstance()->Data[i][j] -1]->Draw((j * _TileWidth)-14, (i * _TileHeight)-14, 255);
		}
	}
}
