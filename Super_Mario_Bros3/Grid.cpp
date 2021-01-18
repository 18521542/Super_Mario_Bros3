#include "Grid.h"
#include "Utils.h"
#include "Game.h"
#include <iostream>
#include <fstream>

Grid::Grid(LPCWSTR filePath, vector<LPGAMEOBJECT>* listObject)
{
	CGame::GetInstance()
	this->listObjectToCheckCollision = listObject;
	for (size_t i = 0; i < listObject->size(); i++)
	{
		if (listObject[i] != mario)
			grid->DevideObjectIntoCell(objects[i]);
	}
	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section;

	char str[1024];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
		case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

}

void Grid::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	//DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	cellWidth = atoi(tokens[0].c_str());
	columns = atoi(tokens[1].c_str());
	rows = atoi(tokens[2].c_str());

	data = new Cell*[columns];
	for (int i = 0; i < columns; i++)
	{
		data[i] = new Cell[rows];
	}
}

void Grid::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	if (tokens.size() < 1) return; // skip invalid lines

	int cellX = atoi(tokens[0].c_str());
	int cellY = atoi(tokens[1].c_str());
	int objectId;

	for (int i = 2; i < tokens.size(); i++)
	{
		objectId = atoi(tokens[i].c_str());
		data[cellX][cellY].Add(listObjectToCheckCollision->at(objectId), objectId);
	}
}

void Grid::Load(LPCWSTR filePath, vector<LPGAMEOBJECT>* listObject)
{
	this->listObjectToCheckCollision = listObject;

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section;

	char str[MAX_GRID_LINE];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
		case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

}

void Grid::GetListObjectsOfCell(vector<LPGAMEOBJECT>* listObject, int camX, int camY)
{
	/*listObjectInCam->clear();
	for (size_t i = 0; i < listObject->size(); i++) 
	{
		if (listObject->at(i)->ObjectInCam()) {
			listObjectInCam->push_back(listObject->at(i));
		}
		
	}*/
	listObject->clear();
	/*for (size_t i = 0; i < listObjectInCam->size(); i++)
	{
		listObject->push_back(listObjectInCam->at(i));
	}*/


	int left, top, right, bottom;
	int i, j, k;
	left = camX / cellWidth;
	top = camY / cellWidth;
	right = left +2;
	bottom = top + 2;

	DebugOut(L"\n======================");
	DebugOut(L"\nLeft: %i", left);
	DebugOut(L"\nTop: %i", top);
	DebugOut(L"\nRight: %i", right);
	DebugOut(L"\nBottom: %i", bottom);
	//DebugOut(L"\n CGame::GetInstance()->GetScreenWidth() / cellWidth = %i", CGame::GetInstance()->GetScreenWidth() / cellWidth);
	LPGAMEOBJECT obj;

	if (right < 0 || left > columns || bottom < 0 && top > rows)
	{
		return;
	}

	if (right > columns)
	{
		right = columns;
	}
	if (bottom > rows)
	{
		bottom = rows;
	}
	if (left < 0)
	{
		left = 0;
	}
	if (top > 0)
	{
		top = 0;
	}

	Cell* cell;

	//Get object by cells
	for (i = left; i <= right; i++)
	{
		for (j = top; j <= bottom; j++)
		{
			cell = &data[i][j];
			if (cell)
			{
				if (data[i][j].GetListObjects().size() != 0)
				{
					for (k = 0; k < data[i][j].GetListObjects().size(); k++)
					{
						obj = data[i][j].GetListObjects().at(k);
						if (CheckObjectId(listObject, obj)) {
							listObject->push_back(obj);
						}
					}
				}
			}

		}
	}
}

void Grid::Unload()
{
	if (data)
	{
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				data[i][j].Unload();
			}
		}
		delete data;
		data = NULL;
	}
}