#pragma once

#include <string>
#include <iostream>

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
public:
	CNPC(void);
	~CNPC(void);

	void setName(string name);
	void setPosX(float x);
	void setPosZ(float z);
	void setSpeed(float speed);
	void setDirection(int state);
	void SetRot(float rot);

	void TestAIPath(void);
	
	string getName(void);
	float getPosX(void);
	float getPosZ(void);
	float getSpeed(void);
	float getRot(void);
	int getDirection(void);
};

