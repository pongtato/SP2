#include <iostream>

#include "RenderPos.h"
#include "Item.h"
#include "NPC.h"

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#pragma once
class CReadFromText
{
private:
	vector<CRenderPos*>my_list;
	vector<CItem* >item_list;
public:
	CReadFromText(void);
	~CReadFromText(void);
	CRenderPos* GetRenderPos(const int selection);
	CItem* GetRenderPosItem(int selection);
	void ReadTextFile(const char* filename);
	void ReadTextFileItem(const char* filename);
	void ResetData(void);
	int ReturnListSize(void);

};

