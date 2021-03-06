/****************************************************************************/
/*!
\file CollisionBounds.cpp
\Author SP2 Group 4
\par
\brief
Codes for collision checking.
*/
/****************************************************************************/
#include "CollisionBounds.h"

/****************************************************************************/
/*!
\brief
Default constructor
*/
/****************************************************************************/
CCollisionBounds::CCollisionBounds(void)
{
}

/****************************************************************************/
/*!
\brief
Default destructor
*/
/****************************************************************************/
CCollisionBounds::~CCollisionBounds(void)
{
}
/****************************************************************************/
/*!
\brief

\param x - x-coordinate
\param y - y-coordinate
\param z - z-coordinate
\param OffsetX - offset distance from x-coordinate
\param OffsetY - offset distance from y-coordinate
\param OffsetZ - offset distance from z-coordinate
*/
/****************************************************************************/
void CCollisionBounds::SetCollisionBounds(float x, float y, float z, float OffsetX, float OffsetY, float OffsetZ)
{
	//Calulate top left of top section cube
	TTL.x = x-OffsetX;
	TTL.y = y+OffsetY;
	TTL.z = z-OffsetZ;

	//Calulate top right of top section cube
	TTR.x = x+OffsetX;
	TTR.y = y+OffsetY;
	TTR.z = z-OffsetZ;

	//Calulate bottom left of top section cube
	TBL.x = x-OffsetX;
	TBL.y = y+OffsetY;
	TBL.z = z+OffsetZ;

	//Calulate bottom left of top section cube
	TBR.x = x+OffsetX;
	TBR.y = y+OffsetY;
	TBR.z = z+OffsetZ;

	//Calulate top left of top section cube
	BTL.x = x-OffsetX;
	BTL.y = y-OffsetY;
	BTL.z = z-OffsetZ;

	//Calulate top right of top section cube
	BTR.x = x+OffsetX;
	BTR.y = y-OffsetY;
	BTR.z = z-OffsetZ;

	//Calulate bottom left of top section cube
	BBL.x = x-OffsetX;
	BBL.y = y-OffsetY;
	BBL.z = z+OffsetZ;

	//Calulate bottom left of top section cube
	BBR.x = x+OffsetX;
	BBR.y = y-OffsetY;
	BBR.z = z+OffsetZ;
}

