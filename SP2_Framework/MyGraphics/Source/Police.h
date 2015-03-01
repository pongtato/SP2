#pragma once
#include <string>
#include <iostream>

using namespace std;

class CPolice
{
public:
	CPolice(void);
	~CPolice(void);

	float PoliceXMove;
	float PoliceZMove;
	float PoliceRotate;
	float FinalPosX;
	float FinalPosZ;
	float WheelRot;
	bool arrived;

	void PoliceDrive(float x, float y, float z,float zOff, float xOff, float rot);
};

