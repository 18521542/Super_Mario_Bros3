#pragma once
#include<vector>
#include "Sprites.h"
class CTileMap
{
	CTileMap* _instance;

	int pixel;
	int idCell;
	int x;				
	int y;				

	int Col;				
	int ColToRead;		
	int Row;				
	int RowToRead;		

	int mapWidth;			
	int mapHeight;			

	int cellWidth;			
	int cellHeight;			

	vector<vector<int>> cellId;

	CSprites* sprites;
	LPCWSTR filePath;
	LPCWSTR bgImagePath;
public:
	CTileMap() {};
	CTileMap(int pixel, LPCWSTR bgImagePath, LPCWSTR filePath, int numCol, int numRow, int numColToRead, int numRowToRead, int idCell);
	void LoadMap();
	void Render();
	void Render(int x);
	void UnLoad() { sprites->Clear(); };

	CTileMap* GetInstance();
};