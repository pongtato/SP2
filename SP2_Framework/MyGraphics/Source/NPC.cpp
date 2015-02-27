#include "NPC.h"


CNPC::CNPC(void)
{
	CharName = "";
	CharPosX = 0;
	CharPosZ = 0;
	CharSpeed = 0;
	CharRot = 90;
	Path = 0;

	srand((unsigned) time(NULL));
	RandomCourse = rand() % 10 + 1;
	shop = true;
	take = false;
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

CNPC* CNPC::GetNPCPos(const int selection)
{
	return this->npc_list[selection];
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
				CNPC * new_pos = new CNPC();
				for ( int i = 0; i < 16; ++i)
				{
					new_pos->setCheckpoints(i,stof(data[i].c_str()));
				}
				npc_list.push_back( new_pos );
			}
		}
	}
	// Close file
	file.close();
}

void CNPC::ShopPathing(int n, bool idle)
{
	if(idle == false)
	{
	// 1st node
	if ( getPosX() <= -75 && getPosZ() >= 25)
	{
		if(GetNPCPos(n)->getCheckpoints(0) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(0));
	}

	// 2nd node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() >= 25)
	{
		if(GetNPCPos(n)->getCheckpoints(1) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(1));
	}

	// 3rd node
	else if( getPosX() >= 0 && getPosX() < 2 && getPosZ() >= 25)
	{
		if(GetNPCPos(n)->getCheckpoints(2) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(2));
	}

	// 4th node
	else if ( getPosX() >= 35 && getPosZ() >= 25)
	{
		if(GetNPCPos(n)->getCheckpoints(3) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(3));
	}

	// 5th node
	else if ( getPosX() <= -75 && getPosZ() <= 7 && getPosZ() > -8)
	{
		if(GetNPCPos(n)->getCheckpoints(4) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(4));
	}

	// 6th node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() <= 7 && getPosZ() > 5)
	{
		if(GetNPCPos(n)->getCheckpoints(5) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(5));
	}
	// 7th node
	else if ( getPosX() >= 0 && getPosX() < 2 && getPosZ() <= 7 && getPosZ() > 5)
	{
		if(GetNPCPos(n)->getCheckpoints(6) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(6));
	}

	// 8th node
	else if ( getPosX() >= 35 && getPosZ() <= 7 && getPosZ() > 5)
	{
		if(GetNPCPos(n)->getCheckpoints(7) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(7));
	}

	// 9th node
	else if ( getPosX() <= -75 && getPosZ() <= -8 && getPosZ() > -10)
	{
		if(GetNPCPos(n)->getCheckpoints(8) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(8));
	}

	// 10th node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() <= -8 && getPosZ() > -10)
	{
		if(GetNPCPos(n)->getCheckpoints(9) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(9));
	}
	// 11th node
	else if ( getPosX() >= 0 && getPosX() < 2 &&  getPosZ() <= -8 && getPosZ() > -10)
	{
		if(GetNPCPos(n)->getCheckpoints(10) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(10));
	}

	// 12th node
	else if ( getPosX() >= 35 && getPosZ() <= -8 && getPosZ() > -10)
	{
		if(GetNPCPos(n)->getCheckpoints(11) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(11));
	}

	// 13th node
	else if ( getPosX() <= -75 && getPosZ() <= -25)
	{
		if(GetNPCPos(n)->getCheckpoints(12) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(12));
	}

	// 14th node
	else if ( getPosX() >= -35 && getPosX() < -33 && getPosZ() <= -25)
	{
		if(GetNPCPos(n)->getCheckpoints(13) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(13));
	}
	// 15th node
	else if ( getPosX() >= 0 && getPosX() < 2 && getPosZ() <= -25)
	{
		if(GetNPCPos(n)->getCheckpoints(14) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(14));
	}

	// 16th node
	else if ( getPosX() >= 35 && getPosZ() <= -25)
	{
		if(GetNPCPos(n)->getCheckpoints(15) != 0)
			setPath(GetNPCPos(n)->getCheckpoints(15));
	}

	//left
	if ( getPath() == 1)
	{
		if(getRot() != 270)
		{
			turnleft();
			RandomCourse = rand() % 10;
		}
		else
		{
			AIShop();
			float temp = getPosX() - 0.2;
			setPosX(temp);
		}
	}
	//right
	else if ( getPath() == 2)
	{
		if(getRot() != 90)
		{
			turnright();
			RandomCourse = rand() % 10;
		}
		else
		{
			AIShop();
			float temp = getPosX() + 0.2;
			setPosX(temp);
		}
	}
	//up
	else if ( getPath() == 3)
	{
		if(getRot() != 180)
		{
			turnup();
			RandomCourse = rand() % 10;
		}
		else
		{
			float temp = getPosZ() - 0.2;
			setPosZ(temp);
		}
	}
	//down
	else if ( getPath() == 4)
	{
		if(getRot() != 0)
		{
			turndown();
			RandomCourse = rand() % 10;
		}
		else
		{
			float temp = getPosZ() + 0.2;
			setPosZ(temp);
		}
	}
	if(getPosX() > 38 && getPosZ() < 25)
	{
		setPosX(38);
	}
	}
}

void CNPC::turnleft(void)
{
	if(getRot() != 270)
	{
		if(getRot() >= 360)
		{
			int temp2 = getRot() - 360;
			SetRot(temp2);
		}
		else if(getRot() == -90)
		{
			SetRot(270);
		}
		else if(getRot() < 90)
		{
			int temp2 = getRot() - 3;
			SetRot(temp2);
		}
		else if( getRot() > 90)
		{
			int temp2 = getRot() + 3;
			SetRot(temp2);
		}
	}
}

void CNPC::turnright(void)
{
	if(getRot() != 90)
	{
		if(getRot() >= 450)
		{
			int temp2 = getRot() - 360;
			SetRot(temp2);
		}
		else if(getRot() < 0)
		{
			int temp2 = getRot() + 360;
			SetRot(temp2);
		}
		else if(getRot() < 270)
		{
			int temp2 = getRot() - 3;
			SetRot(temp2);
		}
		else // getRot() > 270
		{
			int temp2 = getRot() + 3;
			SetRot(temp2);
		}
	}
}

void CNPC::turnup(void)
{
	if(getRot() != 180)
	{
		if(getRot() >= 360)
		{
			int temp2 = getRot() - 360;
			SetRot(temp2);
		}
		else if(getRot() < 0)
		{
			int temp2 = getRot() + 360;
			SetRot(temp2);
		}
		else if(getRot() < 180)
		{
			int temp2 = getRot() + 3;
			SetRot(temp2);
		}
		else // getRot() > 180
		{
			int temp2 = getRot() - 3;
			SetRot(temp2);
		}
	}
}

void CNPC::turndown(void)
{
	if(getRot() != 0)
	{
		if(getRot() >= 360)
		{
			int temp2 = getRot() - 360;
			SetRot(temp2);
		}
		else if(getRot() == 270)
		{
			int temp2 = getRot() - 360;
			SetRot(temp2);
		}
		else if(getRot() < 0)
		{
			int temp2 = getRot() + 3;
			SetRot(temp2);
		}
		else // getRot() == 270
		{
			int temp2 = getRot() - 3;
			SetRot(temp2);
		}
	}
}

void CNPC::AIShop(void)
{
	if(RandomCourse == 1 || RandomCourse == 2) // 7
	{
		if(getPosX() < -60 && getPosX() >= -60.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < -26 && getPosX() >= -26.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < 8.3 && getPosX() >= 8 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else
		{
			shop = true;
			take = false;
		}
	}
	else if(RandomCourse == 3 || RandomCourse == 4) // 14
	{
		if(getPosX() < -54 && getPosX() >= -54.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < -20 && getPosX() >= -20.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < 14.3 && getPosX() >= 14 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else
		{
			shop = true;
			take = false;
		}
	}
	else if(RandomCourse == 5 || RandomCourse == 6) // 21
	{
		if(getPosX() < -48 && getPosX() >= -48.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < -14 && getPosX() >= -14.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < 20.3 && getPosX() >= 20 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else
		{
			shop = true;
			take = false;
		}
	}
	else if(RandomCourse == 7 || RandomCourse == 8)
	{
		if(getPosX() < -42 && getPosX() >= -42.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < -8 && getPosX() >= -8.3 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else if(getPosX() < 26.3 && getPosX() >= 26 && shop == true)
		{
			turn();
			shop = false;
			RandomCourse = rand() % 10;
		}
		else
		{
			shop = true;
			take = false;
		}
	}
}

void CNPC::turn(void)
{
	if(getPosZ() >= 23)
	{
		turnup();
		take = true;
		int temp = getRot();
		if(temp > 180)
		{
			if(getRot() != 270)
			{
				turnleft();
			}
			else
			{
				float temp = getPosX() - 0.2;
				setPosX(temp);
			}
		}
		else
		{
			if(getRot() != 90)
			{
				turnright();
			}
			else
			{
				float temp = getPosX() + 0.2;
				setPosX(temp);
			}
		}
	}
	else if(getPosZ() <= 7 && getPosZ() > 5)
	{
		turnup();
		take = true;
		//turndown();
		int temp = getRot();
		turndown();
		if(temp > 180)
		{
			if(getRot() != 270)
			{
				turnleft();
			}
			else
			{
				float temp = getPosX() - 0.2;
				setPosX(temp);
			}
		}
		else
		{
			if(getRot() != 90)
			{
				turnright();
			}
			else
			{
				float temp = getPosX() + 0.2;
				setPosX(temp);
			}
		}
	}
	else if(getPosZ() <= -8 && getPosZ() > -10)
	{
		turnup();
		take = true;
		//turndown();
		int temp = getRot();
		turndown();
		if(temp > 180)
		{
			if(getRot() != 270)
			{
				turnleft();
			}
			else
			{
				float temp = getPosX() - 0.2;
				setPosX(temp);
			}
		}
		else
		{
			if(getRot() != 90)
			{
				turnright();
			}
			else
			{
				float temp = getPosX() + 0.2;
				setPosX(temp);
			}
		}
	}
	else if(getPosZ() <= -25)
	{
		turndown();
		take = true;
		int temp = getRot();
		if(temp > 180)
		{
			if(getRot() != 270)
			{
				turnleft();
			}
			else
			{
				float temp = getPosX() - 0.2;
				setPosX(temp);
			}
		}
		else
		{
			if(getRot() != 90)
			{
				turnright();
			}
			else
			{
				float temp = getPosX() + 0.2;
				setPosX(temp);
			}
		}
	}
}

void CNPC::setTake(bool temp_take)
{
	take = temp_take;
}

bool CNPC::getTake(void)
{
	return take;
}

void CNPC::setCheckpoints(int n, float point)
{
	Checkpoints[n] = point;
}

float CNPC::getCheckpoints(int n)
{
	return Checkpoints[n];
}

void CNPC::setCurrentState(int state)
{
	CurrentState = state;
}

int CNPC::getCurrentState(void)
{
	return CurrentState;
}



