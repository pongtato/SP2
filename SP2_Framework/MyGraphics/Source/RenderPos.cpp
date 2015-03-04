/****************************************************************************/
/*!
\file RenderPos.cpp
\Author SP2 Group 4
\par
\brief
Codes to rendering the positions of objects from text files.
*/
/****************************************************************************/
#include "RenderPos.h"


CRenderPos::CRenderPos(void)
{
}


CRenderPos::~CRenderPos(void)
{
}

void CRenderPos::setRotation(float rotate,int x,int y, int z)
{
	rotation = rotate;
	RX = x;
	RY = y;
	RZ = z;
}

void CRenderPos::setTranslation(float x, float y, float z)
{
	translationX = x;
	translationY = y;
	translationZ = z;
}

void CRenderPos::setScale(float x, float y, float z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;
}

void CRenderPos::setName(string name)
{
	modelName = name;
}

float CRenderPos::getRotation(void)
{
	return rotation;
}

int CRenderPos::getRX(void)
{
	return RX;
}

int CRenderPos::getRY(void)
{
	return RY;
}

int CRenderPos::getRZ(void)
{
	return RZ;
}

float CRenderPos::getScaleX(void)
{
	return scaleX;
}

float CRenderPos::getScaleY(void)
{
	return scaleY;
}

float CRenderPos::getScaleZ(void)
{
	return scaleZ;
}

float CRenderPos::getTranslationX(void)
{
	return translationX;
}

float CRenderPos::getTranslationY(void)
{
	return translationY;
}

float CRenderPos::getTranslationZ(void)
{
	return translationZ;
}

string CRenderPos::getName(void)
{
	return modelName;
}