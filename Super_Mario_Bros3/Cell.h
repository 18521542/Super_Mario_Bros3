#pragma once
#include "GameObject.h"

class Cell {
	vector<LPGAMEOBJECT> listObjects;
	vector<int> idX;
	vector<int> idY;
public:
	void Add(CGameObject* obj, int idX){ listObjects.push_back(obj); this->idX.push_back(idX);  }
	vector<LPGAMEOBJECT> GetListObjects() { return listObjects; }
	void Unload() { listObjects.clear(); }

};