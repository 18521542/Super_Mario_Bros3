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
	for (int i = 0; i < RowOfMap; i++) {
		for (int j = 0; j < ColumnsOfMap; j++) {
			if(Data)
				CTileMap::GetInstance()->Tiles[CTileMap::GetInstance()->Data[i][j] -1]->Draw(j * _TileHeight, i * _TileWidth, 255);
		}
	}
}
