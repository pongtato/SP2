#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CTrolley
{
private:
public:
	CTrolley(void);
	~CTrolley(void);
	float PositionTranslate;
	float LastX;
	float LastZ;
	float LastRotation;
	int ExpansionSize;
	int TrolleyLimit;
	int TrolleyCount;

	void Spawner(void);
	void SetList(float x,float z, float rot, int size);
};

