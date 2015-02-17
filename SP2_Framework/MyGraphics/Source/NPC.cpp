#include "NPC.h"


CNPC::CNPC(void)
{
	CharName = "";
	CharPosX = 0;
	CharPosZ = 0;
	CharSpeed = 0;
	CharRot = 90;
	Path = 0;
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


	if ( getDirection() == 1 && getRot() >= 90)
	{

		if ( getRot() <= 90)
		{
			float temp2 = getRot() + 3;
			SetRot(temp2);
		}
		else 
		{
			float temp = getPosX() + 0.2;
			setPosX(temp);
		}
	}
	else if ( getDirection() == 1 && getRot() <= -180)
	{
		if ( getRot() >= -270 )
		{
			float temp2 = getRot() - 3;
			SetRot(temp2);
		}
		else
		{
		SetRot(90);
		}
	}
	else if ( getDirection() == 2)
	{
		if ( getRot() >= 0 )
		{
			float temp2 = getRot() - 3;
			SetRot(temp2);
		}
		else
		{
			float temp = getPosZ() + 0.2;
			setPosZ(temp);
		}
	}
	else if ( getDirection() == 3)
	{
		if ( getRot() >= -90 )
		{
			float temp2 = getRot() - 3;
			SetRot(temp2);
		}
		else
		{
		float temp = getPosX() - 0.2;
		setPosX(temp);
		}
	}
	else if ( getDirection() == 4)
	{
		if ( getRot() >= -180 )
		{
			float temp2 = getRot() - 3;
			SetRot(temp2);
		}
		else
		{
		float temp = getPosZ() - 0.2;
		setPosZ(temp);
		}
	}
}

void CNPC::setPath(int node)
{
	Path = node;
}

int CNPC::getPath(void)
{
	return Path;
}

void CNPC::ReadTextFilePath(const char* filename)
{
	// Open File
	ifstream file;
	file.open( filename );

	// Read File
	if ( file.is_open() )
	{
		// Reset data before doing anything
		//this->ResetData();

		// Reset File
		file.clear();
		file.seekg( 0, file.beg );

		// File Is Healthy
		while ( file.good() )
		{
			// Get Data Per Line
			string aLineOfText = "";
			string token = "";

			vector<string> data;
			data.clear();
			
			getline( file, aLineOfText );
			//aLineOfText.erase(aLineOfText.find(' '),1);
			istringstream iss( aLineOfText );

			while ( getline( iss, token, ',' ) || getline( iss, token, '/' ))
			data.push_back( token );

			if ( data[0] != "" )
			{
				//CNPC * new_pos = new CNPC();
				for ( int i = 0; i < 16; ++i)
				{
					setCheckpoints(i,stof(data[i].c_str()));
				}
			}
		}
	}
	// Close file
	file.close();
}

void CNPC::ShopPathing(void)
{
	// 1st node
	if ( getPosX() <= -75 && getPosZ() >= 25)
	{
		if(getCheckpoints(0) != 0)
			setPath(getCheckpoints(0));
	}

	// 2nd node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() >= 25)
	{
		if(getCheckpoints(1) != 0)
			setPath(getCheckpoints(1));
	}

	// 3rd node
	else if( getPosX() >= 0 && getPosX() < 2 && getPosZ() >= 25)
	{
		if(getCheckpoints(2) != 0)
			setPath(getCheckpoints(2));
	}

	// 4th node
	else if ( getPosX() >= 35 && getPosZ() >= 25)
	{
		if(getCheckpoints(3) != 0)
			setPath(getCheckpoints(3));
	}

	// 5th node
	else if ( getPosX() <= -75 && getPosZ() <= 7 && getPosZ() > -8)
	{
		if(getCheckpoints(4) != 0)
			setPath(getCheckpoints(4));
	}

	// 6th node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() <= 7 && getPosZ() > 5)
	{
		if(getCheckpoints(5) != 0)
			setPath(getCheckpoints(5));
	}
	// 7th node
	else if ( getPosX() >= 0 && getPosX() < 2 && getPosZ() <= 7 && getPosZ() > 5)
	{
		if(getCheckpoints(6) != 0)
			setPath(getCheckpoints(6));
	}

	// 8th node
	else if ( getPosX() >= 35 && getPosZ() <= 7 && getPosZ() > 5)
	{
		if(getCheckpoints(7) != 0)
			setPath(getCheckpoints(7));
	}

	// 9th node
	else if ( getPosX() <= -75 && getPosZ() <= -8 && getPosZ() > -10)
	{
		if(getCheckpoints(8) != 0)
			setPath(getCheckpoints(8));
	}

	// 10th node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() <= -8 && getPosZ() > -10)
	{
		if(getCheckpoints(9) != 0)
			setPath(getCheckpoints(9));
	}
	// 11th node
	else if ( getPosX() >= 0 && getPosX() < 2 &&  getPosZ() <= -8 && getPosZ() > -10)
	{
		if(getCheckpoints(10) != 0)
			setPath(getCheckpoints(10));
	}

	// 12th node
	else if ( getPosX() >= 35 && getPosZ() <= -8 && getPosZ() > -10)
	{
		if(getCheckpoints(11) != 0)
			setPath(getCheckpoints(11));
	}

	// 13th node
	else if ( getPosX() <= -75 && getPosZ() <= -25)
	{
		if(getCheckpoints(12) != 0)
			setPath(getCheckpoints(12));
	}

	// 14th node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() <= -25)
	{
		if(getCheckpoints(13) != 0)
			setPath(getCheckpoints(13));
	}
	// 15th node
	else if ( getPosX() >= 0 && getPosX() < 2 && getPosZ() <= -25)
	{
		if(getCheckpoints(14) != 0)
			setPath(getCheckpoints(14));
	}

	// 16th node
	else if ( getPosX() >= 35 && getPosZ() <= -25)
	{
		if(getCheckpoints(15) != 0)
			setPath(getCheckpoints(15));
	}

	//left
	if ( getPath() == 1)
	{
		float temp = getPosX() - 0.2;
		setPosX(temp);
		SetRot(-90);
	}
	//right
	else if ( getPath() == 2)
	{
		float temp = getPosX() + 0.2;
		setPosX(temp);
		SetRot(90);
	}
	//up
	else if ( getPath() == 3)
	{
		float temp = getPosZ() - 0.2;
		setPosZ(temp);
		SetRot(180);
	}
	//down
	else if ( getPath() == 4)
	{
		float temp = getPosZ() + 0.2;
		setPosZ(temp);
		SetRot(0);
	}
}

void CNPC::setCheckpoints(int n, float point)
{
	Checkpoints[n] = point;
}

float CNPC::getCheckpoints(int n)
{
	return Checkpoints[n];
}




