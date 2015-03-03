#pragma once

#include <string>
#include <iostream>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "Camera3.h"

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
	vector<CNPC*>npc_list;
	//for AI
	bool shop;
	
	

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
	void ShopPathing(int n, bool idle);
	
	string getName(void);
	float getPosX(void);
	float getPosZ(void);
	float getSpeed(void);
	float getRot(void);
	int getDirection(void);
	int getPath(void);
	float getCheckpoints(int n);
	int getCurrentState(void);

	//AI NPC
	int maxItem;
	bool cashIn;
	bool exitMart;
	bool enterMart;
	int RandNumGen(int minNum, int maxNum);
	void Checkout();
	void Steal();
	void setShop(bool input);
	bool getShop();
	void respawn();

	void ReadTextFilePath(const char* filename);
	CNPC* GetNPCPos(const int selection);
};

