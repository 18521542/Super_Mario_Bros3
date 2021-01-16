#pragma once
#include "Cell.h"
class Grid {
	int rows, columns;
	Cell** data;

	vector<LPGAMEOBJECT>* listObjectToCheckCollision;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	Grid(LPCWSTR filePath, vector<LPGAMEOBJECT>* listObject);
	void GetListObjectsOfCell(float camX,float camY );
	void Load(LPCWSTR filePath, vector<LPGAMEOBJECT>* listObject);
	void Unload();
};