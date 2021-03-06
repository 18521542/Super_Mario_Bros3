#pragma once
#include "Cell.h"

#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2
#define MAX_GRID_LINE 1024
class Grid {
	int rows, columns;
	Cell** data;
	int cellWidth;

	vector<LPGAMEOBJECT>* listObjectInCam;
	vector<LPGAMEOBJECT>* listObjectToCheckCollision;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);
	
public:
	/*void DevideObjectIntoCell(CGameObject* obj) 
	{
		int indexX, indexY;
		float X, Y;
		obj->GetPosition(X, Y);
		indexX = (int)X / cellWidth;
		indexY = (int) Y / cellWidth;
		data[indexX][indexY].Add(obj, 1);
	}*/
	vector<LPGAMEOBJECT>* GetList() { return this->listObjectToCheckCollision; }
	int Rows() { return this->rows; }
	int Columns() { return this->columns; }
	Grid(LPCWSTR filePath, vector<LPGAMEOBJECT>* listObject);
	void GetListObjectsOfCell(vector<LPGAMEOBJECT>* listObject, int camX, int camY);
	bool CheckObjectId(vector<LPGAMEOBJECT>* listObjects, LPGAMEOBJECT obj)
	{
		for (size_t i = 0; i < listObjects->size(); i++)
		{
			if (listObjects->at(i) == obj) {
				return false;
			}
		}
		return true;
	}
	void Load(LPCWSTR filePath, vector<LPGAMEOBJECT>* listObject);
	void GetObjectId(int cellX, int cellY) {
		
	}
	void Unload();
};