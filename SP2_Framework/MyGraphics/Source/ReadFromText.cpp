#include "ReadFromText.h"


CReadFromText::CReadFromText(void)
{
	my_list.clear();
}

CReadFromText::~CReadFromText(void)
{
	this->ResetData();
}

CRenderPos* CReadFromText::GetRenderPos(const int selection)
{
	return this->my_list[selection];
}

CItem* CReadFromText::GetRenderPosItem(int selection)
{
	return this->item_list[selection];
}

CItem* CReadFromText::GetRenderPosPolice(int selection)
{
	return this->Police_list[selection];
}

void CReadFromText::ResetData(void)
{
	// Release memory 
	for ( int i=0; i < my_list.size(); i ++ )
	{
		 delete my_list[i];
		 my_list[i] = NULL;
	}
	my_list.clear();
	my_list.shrink_to_fit();
}

void CReadFromText::ReadTextFile(const char* filename)
{
	// Open File
	ifstream file;
	file.open( filename );

	// Read File
	if ( file.is_open() )
	{
		// Reset data before doing anything
		this->ResetData();

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
				CRenderPos * new_pos = new CRenderPos();
				new_pos->setName(data[0]);
				new_pos->setTranslation( stof(data[1].c_str()), stof(data[2].c_str()), stof(data[3].c_str()));
				new_pos->setRotation( stof(data[4].c_str()), atoi(data[5].c_str()), atoi(data[6].c_str()), atoi(data[7].c_str()));
				new_pos->setScale( stof(data[8].c_str()), stof(data[9].c_str()), stof(data[10].c_str()));
				my_list.push_back( new_pos );
			}
		}
	}
	// Close file
	file.close();
}

void CReadFromText::ReadTextFileItem(const char* filename)
{
	// Open File
	ifstream file;
	file.open( filename );

	// Read File
	if ( file.is_open() )
	{
		// Reset data before doing anything
		this->ResetData();

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
				CItem * item_pos = new CItem();
				item_pos->setItemName(data[0]);
				item_pos->setItemTranslation( stof(data[1].c_str()), stof(data[2].c_str()), stof(data[3].c_str()));
				item_pos->setItemRotation( stof(data[4].c_str()), atoi(data[5].c_str()), atoi(data[6].c_str()), atoi(data[7].c_str()));
				item_pos->setItemScale( stof(data[8].c_str()), stof(data[9].c_str()), stof(data[10].c_str()));
				item_pos->setItemPrice(stof(data[11].c_str()));
				item_pos->setItemAvailable(stof(data[12].c_str()));
				item_list.push_back( item_pos );
			}
		}
	}
	// Close file
	file.close();
}

void CReadFromText::ReadTextFilePolice(const char* filename)
{
	// Open File
	ifstream file;
	file.open( filename );

	// Read File
	if ( file.is_open() )
	{
		// Reset data before doing anything
		this->ResetData();

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
				CItem * item_pos = new CItem();
				item_pos->setItemName(data[0]);
				item_pos->setItemTranslation( stof(data[1].c_str()), stof(data[2].c_str()), stof(data[3].c_str()));
				item_pos->setItemRotation( stof(data[4].c_str()), atoi(data[5].c_str()), atoi(data[6].c_str()), atoi(data[7].c_str()));
				item_pos->setItemScale( stof(data[8].c_str()), stof(data[9].c_str()), stof(data[10].c_str()));
				item_pos->setItemPrice(stof(data[11].c_str()));
				item_pos->setItemAvailable(stof(data[12].c_str()));
				item_pos->ShootCD = 0;
				item_pos->RespawnCD = 0;
				Police_list.push_back( item_pos );
			}
		}
	}
	// Close file
	file.close();
}

int CReadFromText::ReturnListSize(void)
{
	return item_list.size();
}

int CReadFromText::ReturnReadListSize(void)
{
	return my_list.size();
}

int CReadFromText::ReadTextFilePoliceSize(void)
{
	return Police_list.size();
}


