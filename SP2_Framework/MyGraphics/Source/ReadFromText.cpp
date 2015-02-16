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

void CReadFromText::ResetData(void)
{
	// Release memory 
	for ( int i=0; i < my_list.size(); i ++ )
	{
		 delete my_list[i];
		 my_list[i] = NULL;
		 cout << " Deleted " << endl;
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


