#include "TileMap.h"
#include "Textures.h"
#include "Mario.h"
#include <iostream>
#include <fstream>

using namespace std;

CTileMap::CTileMap(int pixel, LPCWSTR bgImagePath, LPCWSTR filePath, int numCol, int numRow, int numColToRead, int numRowToRead, int idCell)
{
	sprites = CSprites::GetInstance();
	this->pixel = pixel;
	this->bgImagePath = bgImagePath;
	this->filePath = filePath;
	this->Col = numCol;
	this->Row = numRow;
	this->ColToRead = numColToRead;
	this->RowToRead = numRowToRead;
	this->idCell = idCell;

	LoadMap();
}

void CTileMap::LoadMap()
{
	CTextures* textures = CTextures::GetInstance();
	textures->Add(70, bgImagePath, D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texTileMap = textures->Get(70);

	for (int i = 0; i < RowToRead; i++)
	{
		for (int j = 0; j < ColToRead; j++)
		{
			sprites->Add(idCell, pixel * j, pixel * i, pixel + pixel * j, pixel + pixel * i, texTileMap);
			idCell++;
		}
	}

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int value;
	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		vector<string> tokens = split(line, " ");
		vector<int> lineOfCell;
		DebugOut(L"--> %s\n", ToWSTR(line).c_str());

		for (int i = 0; i < tokens.size(); i++)	// why i+=2 ?  sprite_id | frame_time  
		{
			if (atoi(tokens[i].c_str())) lineOfCell.push_back(atoi(tokens[i].c_str()));
		}
		cellId.push_back(lineOfCell);
	}

	f.close();
}

void CTileMap::Render()
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			sprites->Get(cellId[i][j])->Draw(j * pixel, i * pixel);
		}
	}
}

void CTileMap::Render(int x)
{
	int start, finish;
	start = x / pixel - 12;
	finish = start + 29;

	if (start < 0)
	{
		start = 0;
	}
	if (finish > Col)
	{
		finish = Col;
	}
	for (int i = 0; i < Row; i++)
	{
		for (int j = start; j < finish; j++)
		{
			sprites->Get(cellId[i][j])->Draw(j * pixel, i * pixel);
		}
	}
}

CTileMap* CTileMap::GetInstance()
{
	if (_instance == NULL) _instance = new CTileMap();
	return _instance;
}
