/****************************************************************************/
/*!
\file Trolley.h
\Author SP2 Group 4
\par
\brief
Class to define functions and variables for the Trolley.
*/
/****************************************************************************/
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

