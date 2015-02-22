#include "Item.h"


CItem::CItem(void)
{
	itemName = "";
	itemPrice = 0;
	itemAvailable = false;
}


CItem::~CItem(void)
{
}

void CItem::setItemRotation(float rotate,int x,int y, int z)
{
	rotation = rotate;
	RX = x;
	RY = y;
	RZ = z;
}

void CItem::setItemTranslation(float x, float y, float z)
{
	translationX = x;
	translationY = y;
	translationZ = z;
}

void CItem::setItemScale(float x, float y, float z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;
}

float CItem::getItemRotation(void)
{
	return rotation;
}

int CItem::getItemRX(void)
{
	return RX;
}

int CItem::getItemRY(void)
{
	return RY;
}

int CItem::getItemRZ(void)
{
	return RZ;
}

float CItem::getItemScaleX(void)
{
	return scaleX;
}

float CItem::getItemScaleY(void)
{
	return scaleY;
}

float CItem::getItemScaleZ(void)
{
	return scaleZ;
}

float CItem::getItemTranslationX(void)
{
	return translationX;
}

float CItem::getItemTranslationY(void)
{
	return translationY;
}

float CItem::getItemTranslationZ(void)
{
	return translationZ;
}

void CItem::setItemName(string name)
{
	itemName = name;
}

void CItem::setItemPrice(double price)
{
	itemPrice = price;
}

void CItem::setItemAvailable(bool bought)
{
	itemAvailable = bought;
}

string CItem::getItemName(void)
{
	return itemName;
}

double CItem::getItemPrice(void)
{
	return itemPrice;
}

bool CItem::getItemAvailability(void)
{
	return itemAvailable;
}

void CItem::setItemCount(int n)
{
	itemCount+=n;
}

int CItem::getItemCount(void)
{
	return itemCount;
}