#pragma once

#include <string>
#include <iostream>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

class CNPC
{
private:
	string CharName;
	float CharSpeed;
	float CharPosX;
	float CharPosZ;
	float CharRot;
	int Direction;
	int Path;
	int CurrentState;
	float Checkpoints[20];
public:
	CNPC(void);
	~CNPC(void);

	void setName(string name);
	void setPosX(float x);
	void setPosZ(float z);
	void setSpeed(float speed);
	void setDirection(int state);
	void SetRot(float rot);
	void setPath(int node);
	void setCheckpoints(int n, float point);
	void setCurrentState(int state);

	void TestAIPath(void);
	void ShopPathing(void);
	
	string getName(void);
	float getPosX(void);
	float getPosZ(void);
	float getSpeed(void);
	float getRot(void);
	int getDirection(void);
	int getPath(void);
	float getCheckpoints(int n);
	int getCurrentState(void);

	void ReadTextFilePath(const char* filename);
};

