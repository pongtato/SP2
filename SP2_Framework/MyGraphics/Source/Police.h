/****************************************************************************/
/*!
\file Police.h
\Author SP2 Group 4
\par
\brief
Defines functions for the police.
*/
/****************************************************************************/
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
	bool escape;

	void PoliceDrive(float x, float y, float z,float zOff, float xOff, float rot);
	void PoliceDriveL(float x, float rot, float z,float zOff, float xOff, float rotOff);
};

