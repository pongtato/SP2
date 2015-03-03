#pragma once

#include <iostream>
#include <string>

using namespace std;

class CItem
{
private:
	string itemName;
	double itemPrice;
	bool itemAvailable;
	float rotation;
	float translationX;
	float translationY;
	float translationZ;
	int RX;
	int RY;
	int RZ;
	float scaleX;
	float scaleY;
	float scaleZ;
	int itemCount;
public:
	CItem(void);
	~CItem(void);

	void setItemName(string name);
	void setItemPrice(double price);
	void setItemAvailable(bool bought);
	void setItemCount(int n);

	string getItemName(void);
	double getItemPrice(void);
	bool getItemAvailability(void);

	void setItemRotation(float a,int x,int y, int z);
	void setItemTranslation(float x, float y, float z);
	void setItemScale(float x, float y, float z);

	float getItemRotation(void);
	float getItemTranslationX(void);
	float  getItemTranslationY(void);
	float  getItemTranslationZ(void);
	int  getItemRX(void);
	int  getItemRY(void);
	int  getItemRZ(void);
	float  getItemScaleX(void);
	float  getItemScaleY(void);
	float  getItemScaleZ(void);
	int getItemCount(void);
	float ShootCD;
	float RespawnCD;
	void sethealth(int hp);
	void resethealth();
};

