#pragma once
#include<vector>
#include "Sprites.h"

#define TILE_WIDTH				16
#define TILE_HEIGHT				16

#define COLUMNS					161
#define ROWS					29

#define COLUMN_OF_TILE_SET		11
#define ROW_OF_TILE_SET			5

#define TOTAL_SPRITES			52
class CTileMap
{	
	static CTileMap* instance;
	int PlayerPosition;	

	int** Data; //.txt

	int _TileWidth;			
	int _TileHeight;	

	int ColumnsOfMap;
	int RowOfMap;

	int ColumnOfTileset;
	int RowOfTileset;

	vector<LPSPRITE> Tiles;
	LPDIRECT3DTEXTURE9	TilesSetFile; //.png

public:
	void LoadTileset(LPCWSTR Path_Of_Tileset_File);
	void LoadDataFromTileset(LPCWSTR Path_Of_Data_File);
	void LoadTile();
	void LoadDetails(int TilesetColumns, int TilesetRows, int MapColumns, int MapRows, int TileWidth, int TileHeight);

	//void Update();
	void Render();


	static CTileMap* GetInstance();
	void Unload() {
		if (Data)
		{
			for (int i = 0; i < ROWS; i++)
			{
				delete Data[i];
			}
			delete Data;
			Data = nullptr;
		}
		Tiles.clear();
		TilesSetFile = NULL;
	}
};