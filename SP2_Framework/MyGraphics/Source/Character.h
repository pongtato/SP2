#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Item.h"
using namespace std;
class CCharacter
{
private:
	double money;
	int health;
	int stamina;
	int bullets;
	float detection;
	vector<CItem* >inven_list;
	int inventoryCap;
	string career;

public:
	CCharacter(void);
	~CCharacter(void);

	void setMoney(double cash);
	void setHealth(int hp);
	void setStamina(int stam);
	void setBullets(int ammo);
	void setDetection(float detect);
	void setInventory(string name, double price);
	void setClass(string job);
	void dropItem(string itemname);
	void sellItems(int n);
	void setInventoryCap(int size);

	double getMoney(void);
	int getHealth(void);
	int getStamina(void);
	int getBullets(void);
	float getDetection(void);
	CItem* getInventory(int n);
	string getClass(void);
	int returnInvenSize();
	int getInventoryCap(void);
	bool unpaiditems;
	bool trolley;
	bool trolleyDrop;
	bool Align;
};

