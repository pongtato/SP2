#include <iostream>

#include "RenderPos.h"
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
	vector<CNPC*>npc_list;
public:
	CReadFromText(void);
	~CReadFromText(void);

	CRenderPos* GetRenderPos(const int selection);
	CNPC* GetRenderPosChar(const int selection);
	void ReadTextFile(const char* filename);
	void ReadTextFilePath(const char* filename);
	void ResetData(void);

};

