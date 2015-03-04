/****************************************************************************/
/*!
\file RenderPos.h
\Author SP2 Group 4
\par
\brief
Main header file for RenderPos.
*/
/****************************************************************************/
#include <iostream>
#include <string>
#pragma once

using namespace std;

class CRenderPos
{
private:
	float rotation;
	float translationX;
	float translationY;
	float translationZ;
	int RX;
	int RY;
	int RZ;
	float scaleX;
	float scaleY;
	float scaleZ;
	string modelName;
public:
	CRenderPos(void);
	~CRenderPos(void);


	void setRotation(float a,int x,int y, int z);
	void setTranslation(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setName(string name);

	float getRotation(void);
	float getTranslationX(void);
	float  getTranslationY(void);
	float  getTranslationZ(void);
	int  getRX(void);
	int  getRY(void);
	int  getRZ(void);
	float  getScaleX(void);
	float  getScaleY(void);
	float  getScaleZ(void);
	string getName(void);
};

