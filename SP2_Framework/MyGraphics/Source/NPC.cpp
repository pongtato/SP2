#include "NPC.h"


CNPC::CNPC(void)
{
	CharName = "";
	CharPosX = 0;
	CharPosZ = 0;
	CharSpeed = 0;
	CharRot = 0;
}


CNPC::~CNPC(void)
{
}

void CNPC::setName(string name)
{
	CharName = name;
}

void CNPC::setPosX(float x)
{
	CharPosX = x;
}

void CNPC::setPosZ(float z)
{
	CharPosZ = z;
}

void CNPC::setSpeed(float speed)
{
	CharSpeed = speed;
}

string CNPC::getName(void)
{
	return CharName;
}

float CNPC::getPosX(void)
{
	return CharPosX;
}

float CNPC::getPosZ(void)
{
	return CharPosZ;
}

float CNPC::getSpeed(void)
{
	return CharSpeed;
}

void CNPC::setDirection(int state)
{
	Direction = state;
}

int CNPC::getDirection(void)
{
	return Direction;
}

void CNPC::SetRot(float rot)
{
	CharRot = rot;
}

float CNPC::getRot(void)
{
	return CharRot;
}

void CNPC::TestAIPath(void)
{
	if ( getPosX() <= 70 && getPosZ() <= 80 )
	{
		setDirection(1);
	}
	else if ( getPosX() >= 90 && getPosZ() <= 80 )
	{
		setDirection(2);
	}
	else if ( getPosX() >= 90 && getPosZ() >= 100 )
	{
		setDirection(3);
	}
	else if ( getPosX() <= 70 && getPosZ() >= 100 )
	{
		setDirection(4);
	}


	if ( getDirection() == 1)
	{
		float temp = getPosX() + 0.2;
		SetRot(90);
		setPosX(temp);
	}
	else if ( getDirection() == 2)
	{
		float temp = getPosZ() + 0.2;
		SetRot(0);
		setPosZ(temp);
	}
	else if ( getDirection() == 3)
	{
		float temp = getPosX() - 0.2;
		SetRot(-90);
		setPosX(temp);
	}
	else if ( getDirection() == 4)
	{
		float temp = getPosZ() - 0.2;
		SetRot(180);
		setPosZ(temp);
	}
}
