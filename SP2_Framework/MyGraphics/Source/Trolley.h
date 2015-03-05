#pragma once
#include <iostream>
#include <vector>
#include <Vector3.h>
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
	float LastXMove;
	float LastZMove;
	float LastRotation;
	int ExpansionSize;
	int TrolleyLimit;
	int TrolleyCount;
	Vector3 DetectBulletPos;

	void Spawner(void);
	void SetList(float itemx,float itemz,float x,float z, float rot, int size);
};

