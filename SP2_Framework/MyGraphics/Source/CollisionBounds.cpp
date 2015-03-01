#include "CollisionBounds.h"


CCollisionBounds::CCollisionBounds(void)
{
}


CCollisionBounds::~CCollisionBounds(void)
{
}

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

