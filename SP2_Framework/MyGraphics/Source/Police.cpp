#include "Police.h"


CPolice::CPolice(void)
{
	PoliceRotate = 0;
	PoliceXMove = 0;
	PoliceZMove = 0;
	arrived = false;
	FinalPosX= 0;
	FinalPosZ = 0;
	escape = false;
}


CPolice::~CPolice(void)
{
}

void CPolice::PoliceDrive(float x, float rot, float z,float zOff, float xOff, float rotOff)
{
	if (z + PoliceZMove >= zOff)
	{
		PoliceZMove-=3;
		WheelRot++;
	}
	else if(rot + PoliceRotate < rotOff)
	{
		PoliceRotate+=3;
	}
	else if (x + PoliceXMove >= xOff)
	{
		PoliceXMove-=3;
		WheelRot++;
	}
	else
	{
		FinalPosX = xOff;
		FinalPosZ = zOff;
		arrived = true;
	}
}

void CPolice::PoliceDriveL(float x, float rot, float z,float zOff, float xOff, float rotOff)
{
	if (z + PoliceZMove >= zOff)
	{
		PoliceZMove-=3;
		WheelRot++;
	}
	else if(rot + PoliceRotate > rotOff)
	{
		PoliceRotate-=3;
	}
	else if (x + PoliceXMove <= xOff)
	{
		PoliceXMove+=3;
		WheelRot++;
	}
	else
	{
		FinalPosX = xOff;
		FinalPosZ = zOff;
		arrived = true;
	}

}
