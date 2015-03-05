#include "Trolley.h"


CTrolley::CTrolley(void)
{
	TrolleyLimit = 3;
	PositionTranslate = 50;
	TrolleyCount = 0;
}


CTrolley::~CTrolley(void)
{
}

void CTrolley::Spawner(void)
{
}

void CTrolley::SetList(float itemx,float itemz,float x,float z, float rot, int size)
{
	LastX = itemx;
	LastZ = itemz;
	LastXMove = x;
	LastZMove = z;
	LastRotation = rot;
	ExpansionSize = size;
}