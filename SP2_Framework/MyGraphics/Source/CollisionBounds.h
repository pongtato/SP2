/****************************************************************************/
/*!
\file CollisionBounds.h
\Author SP2 Group 4
\par
\brief
Vectors for CollisionBounds.cpp.
*/
/****************************************************************************/
#pragma once
#include "Vector3.h"
class CCollisionBounds
{
public:
	CCollisionBounds(void);
	~CCollisionBounds(void);
	//top left of top section cube
	Vector3 TTL;
	//top right of top section cube
	Vector3 TTR;
	//bottom left of top section cube
	Vector3 TBL;
	//botom  right of top section cube
	Vector3 TBR;
	//top left of bottom section cube
	Vector3 BTL;
	//top right of bottom section cube
	Vector3 BTR;
	//bottom left of bottom section cube
	Vector3 BBL;
	//botom  right of bottom section cube
	Vector3 BBR;

	void SetCollisionBounds(float x, float y, float z,float OffsetX,float OffsetY,float OffsetZ);

};

