#include <iostream>

#include "RenderPos.h"

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
public:
	CReadFromText(void);
	~CReadFromText(void);

	CRenderPos* GetRenderPos(const int selection);
	void ReadTextFile(const char* filename);
	void ResetData(void);

};

