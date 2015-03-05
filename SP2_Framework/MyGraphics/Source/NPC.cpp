#include "NPC.h"


CNPC::CNPC(void)
{
	CharName = "";
	CharPosX = 0;
	CharPosZ = 0;
	CharSpeed = 0;
	CharRot = 90;
	Path = 0;
	shop = false;
	maxItem = 0;
	cashIn = false;
	exitMart = false;
	enterMart = false;
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
		if ( getPosX() <= -75 && getPosZ() >= 25 && cashIn == false)
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
		else if ( getPosX() >= 35 && getPosZ() <= -25  && cashIn == false)
		{
			if(GetNPCPos(n)->getCheckpoints(15) != 0)
				setPath(GetNPCPos(n)->getCheckpoints(15));
		}

		//left
		if ( getPath() == 1 && cashIn == false)
		{
			float temp = getPosX() - 0.2;
			setPosX(temp);
			SetRot(-90);
		}
		//right
		else if ( getPath() == 2  && cashIn == false)
		{
			if(shop == false)
			{
				float temp = getPosX() + 0.2;
				setPosX(temp);
				if(getRot() >= 0 && getRot() <= 90)//rotate left after rotating right
				{
					float temp = getRot() + 1;
					SetRot(temp);
				}
				else
				{
					SetRot(90);
				}
			}
			else if(shop == true)// rotate right to get item from shelf
			{
				float temp = getRot() - 1;
				SetRot(temp);
			}
		}
		//up
		else if ( getPath() == 3 && cashIn == false)
		{
			float temp = getPosZ() - 0.2;
			setPosZ(temp);
			SetRot(180);
		}
		//down
		else if ( getPath() == 4 && cashIn == false)
		{
			float temp = getPosZ() + 0.2;
			setPosZ(temp);
			SetRot(0);
		}
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

void CNPC::setCurrentState(int state)
{
	CurrentState = state;
}

int CNPC::getCurrentState(void)
{
	return CurrentState;
}

int CNPC::RandNumGen(int minNum, int maxNum)
{
	int result = 0;
	for(int i = minNum; i <50; i++)
	{
		result = rand() %maxNum;// randomly generate 0 - maxnum
	}
	return result;
}
void CNPC::Checkout()
{
	if(cashIn == true)
	{
		if(getRot() >= 0 && getRot() < 90 && getPosX() < 41)//turn shopper left after shopping to check out
		{
			float temp = getRot() + 1;
			SetRot(temp);
		}
		if(getPosX() < 41)// walk to cashier
		{
			float temp = getPosX() + 0.2;
			setPosX(temp);
		}
		//Put AI Pay Money Animation
		if(getPosX() >= 41)// AI at left most cashier
		{
			if(getRot() <= 90 && getRot() > 0 && getPosZ() <=-8)//turn shopper right after paying money
			{
				float temp = getRot() - 1;
				SetRot(temp);
			}
		}
		if(getPosZ() < 31 && getPosX() >= 41 && getRot() == 0)// walk to area above rightmost cashier
		{
			float temp = getPosZ() + 0.2;
			setPosZ(temp);
		}
		if(getPosZ() >= 31 && getPosX() < 61)// rotate the AI left
		{
			if(getRot() >= 0 && getRot() < 90)
			{
				float temp = getRot() + 1;
				SetRot(temp);
			}
		}
		if(getRot() == 90 && getPosX() < 61 && getPosZ() >= 31)// move to exit door
		{
			float temp = getPosX() + 0.2;
			setPosX(temp);
		}
		//exit door open
		if(getPosX() >= 61 && getPosZ() >= 31)
		{
			exitMart = true;
		}
		//turn right and exit mart
		if(getPosX() >= 61)
		{
			if(getRot() <= 90 && getRot() > 0 && getPosZ() >= 31 && getPosZ() <75)// turn right
			{
				float temp = getRot() - 1;
				SetRot(temp);
			}
		}
		if(getPosX() >= 61 && getPosZ() >= 31 && getPosZ() <= 75 && getRot() == 0)// exit mart
		{
			float temp = getPosZ() + 0.2;
			setPosZ(temp);
		}
		if(getPosX() >= 61 && getPosZ() >= 41)// exit door close
		{
			exitMart = false;
		}
		//turn left
		if(getPosZ() >=75)
		{
			if(getRot() >= 0 && getRot() < 90)
			{
				float temp = getRot() + 1;
				SetRot(temp);
			}
		}
		//walk out of the fence
		if(getPosZ() >=75 && getRot() == 90)
		{
			float temp = getPosX() + 0.2;
			setPosX(temp);
		}
	}
	
	
}
void CNPC::Steal()
{
}
void CNPC::setShop(bool input)
{
	shop = input;
}
bool CNPC::getShop()
{
	return shop;
}
void CNPC::respawn()
{
	cashIn = false;
	maxItem = 0;
	//exactly outside the entrance door
	setPosX(-75);
	setPosZ(75);
	SetRot(90);
	
}



