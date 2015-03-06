/****************************************************************************/
/*!
\file Item.cpp
\Author SP2 Group 4
\par
\brief
Codes for the properties of an item.
*/
/****************************************************************************/
#include "Item.h"

/****************************************************************************/
/*!
\brief
Default constructor
*/
/****************************************************************************/
CItem::CItem(void)
{
	itemName = "";
	itemPrice = 0;
	itemAvailable = false;
	Damaged = false;
}

/****************************************************************************/
/*!
\brief
Default destructor
*/
/****************************************************************************/
CItem::~CItem(void)
{
}
/****************************************************************************/
/*!
\brief
Sets the health of the item
*/
/****************************************************************************/
void CItem::sethealth(int hp)
{
	itemPrice -= hp;
}
/****************************************************************************/
/*!
\brief
Resets the health of the item
*/
/****************************************************************************/
void CItem::resethealth()
{
	itemPrice = 100;
}
/****************************************************************************/
/*!
\brief
Sets the item rotation

\param rotate - degree of rotation
\param x - boolean function for turning in the direction of the x-axis
\param y - boolean function for turning in the direction of the y-axis
\param z - boolean function for turning in the direction of the z-axis
*/
/****************************************************************************/
void CItem::setItemRotation(float rotate,int x,int y, int z)
{
	rotation = rotate;
	RX = x;
	RY = y;
	RZ = z;
}
/****************************************************************************/
/*!
\brief
Sets the item translation

\param x - int for movement towards the x-axis
\param y - int for movement towards the y-axis
\param z - int for movement towards the z-axis
*/
/****************************************************************************/
void CItem::setItemTranslation(float x, float y, float z)
{
	translationX = x;
	translationY = y;
	translationZ = z;
}
/****************************************************************************/
/*!
\brief
Sets the item scale

\param x - int for scaling towards the x-axis
\param y - int for scaling towards the y-axis
\param z - int for scaling towards the z-axis
*/
/****************************************************************************/
void CItem::setItemScale(float x, float y, float z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;
}
/****************************************************************************/
/*!
\brief
Gets the item rotation
*/
/****************************************************************************/
float CItem::getItemRotation(void)
{
	return rotation;
}
/****************************************************************************/
/*!
\brief
Gets the item rotation for the X-axis
*/
/****************************************************************************/
int CItem::getItemRX(void)
{
	return RX;
}
/****************************************************************************/
/*!
\brief
Gets the item rotation for the Y-axis
*/
/****************************************************************************/
int CItem::getItemRY(void)
{
	return RY;
}
/****************************************************************************/
/*!
\brief
Gets the item rotation for the Z-axis
*/
/****************************************************************************/
int CItem::getItemRZ(void)
{
	return RZ;
}
/****************************************************************************/
/*!
\brief
Gets the item scale for the X-axis
*/
/****************************************************************************/
float CItem::getItemScaleX(void)
{
	return scaleX;
}
/****************************************************************************/
/*!
\brief
Gets the item scale for the Y-axis
*/
/****************************************************************************/
float CItem::getItemScaleY(void)
{
	return scaleY;
}
/****************************************************************************/
/*!
\brief
Gets the item rotation for the Z-axis
*/
/****************************************************************************/
float CItem::getItemScaleZ(void)
{
	return scaleZ;
}
/****************************************************************************/
/*!
\brief
Gets the item translation for the X-axis
*/
/****************************************************************************/
float CItem::getItemTranslationX(void)
{
	return translationX;
}
/****************************************************************************/
/*!
\brief
Gets the item translation for the Y-axis
*/
/****************************************************************************/
float CItem::getItemTranslationY(void)
{
	return translationY;
}
/****************************************************************************/
/*!
\brief
Gets the item transltion for the Z-axis
*/
/****************************************************************************/
float CItem::getItemTranslationZ(void)
{
	return translationZ;
}
/****************************************************************************/
/*!
\brief
Sets the item name
*/
/****************************************************************************/
void CItem::setItemName(string name)
{
	itemName = name;
}
/****************************************************************************/
/*!
\brief
Sets the item price
*/
/****************************************************************************/
void CItem::setItemPrice(double price)
{
	itemPrice = price;
}
/****************************************************************************/
/*!
\brief
Checks if the item can be bought
*/
/****************************************************************************/
void CItem::setItemAvailable(bool bought)
{
	itemAvailable = bought;
}
/****************************************************************************/
/*!
\brief
Gets the item name
*/
/****************************************************************************/
string CItem::getItemName(void)
{
	return itemName;
}
/****************************************************************************/
/*!
\brief
Gets the item price
*/
/****************************************************************************/
double CItem::getItemPrice(void)
{
	return itemPrice;
}
/****************************************************************************/
/*!
\brief
Gets the check if item is available
*/
/****************************************************************************/
bool CItem::getItemAvailability(void)
{
	return itemAvailable;
}
/****************************************************************************/
/*!
\brief
Sets the item count
*/
/****************************************************************************/
void CItem::setItemCount(int n)
{
	itemCount+=n;
}
/****************************************************************************/
/*!
\brief
Gets the item count
*/
/****************************************************************************/
int CItem::getItemCount(void)
{
	return itemCount;
}