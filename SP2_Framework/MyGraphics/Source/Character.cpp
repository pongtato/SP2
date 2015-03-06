/****************************************************************************/
/*!
\file Character.cpp
\Author SP2 Group 4
\par
\brief
Codes to the properties of the player character.
*/
/****************************************************************************/
#include "Character.h"
/****************************************************************************/
/*!
\brief
Default constructor
*/
/****************************************************************************/

CCharacter::CCharacter(void)
{
	health = 100;
	stamina = 200;
	money = 100;
	detection = 0;
	bullets = 0;
	inventoryCap = 5;
	career = "Unemployed";
	unpaiditems = false;
	trolley = false;
	trolleyDrop = false;
	Align = false;
	Restart = false;
}

/****************************************************************************/
/*!
\brief
Default destructor
*/
/****************************************************************************/
CCharacter::~CCharacter(void)
{
}
/****************************************************************************/
/*!
\brief
Sets player character health
*/
/****************************************************************************/
void CCharacter::setHealth(int hp)
{
	health -= hp;
}
/****************************************************************************/
/*!
\brief
Sets player character money
*/
/****************************************************************************/
void CCharacter::setMoney(double cash)
{
	money -= cash;
}
/****************************************************************************/
/*!
\brief
Sets player character bullets
*/
/****************************************************************************/
void CCharacter::setBullets(int ammo)
{
	bullets = ammo;
}
/****************************************************************************/
/*!
\brief
Sets player character stamina
*/
/****************************************************************************/
void CCharacter::setStamina(int stam)
{
	stamina -= stam;
}
/****************************************************************************/
/*!
\brief
Sets player character detection
*/
/****************************************************************************/
void CCharacter::setDetection(float detect)
{
	detection = detect;
}
/****************************************************************************/
/*!
\brief
Sets player character class
*/
/****************************************************************************/
void CCharacter::setClass(string job)
{
	career = job;
}
/****************************************************************************/
/*!
\brief
Gets player character money
*/
/****************************************************************************/
double CCharacter::getMoney(void)
{
	return money;
}
/****************************************************************************/
/*!
\brief
Gets player character health
*/
/****************************************************************************/
int CCharacter::getHealth(void)
{
	return health;
}
/****************************************************************************/
/*!
\brief
Gets player character stamina
*/
/****************************************************************************/
int CCharacter::getStamina(void)
{
	return stamina;
}
/****************************************************************************/
/*!
\brief
Gets player character detection
*/
/****************************************************************************/
float CCharacter::getDetection(void)
{
	return detection;
}
/****************************************************************************/
/*!
\brief
Gets player character class
*/
/****************************************************************************/
string CCharacter::getClass(void)
{
	return career;
}
/****************************************************************************/
/*!
\brief
Sets player character inventory

\param name - name of item in inventory
\param price - price of item in inventory
*/
/****************************************************************************/
void  CCharacter::setInventory(string name, double price)
{
	CItem * inven = new CItem();
	inven->setItemName(name);
	inven->setItemPrice(price);
	inven_list.push_back(inven);
}
/****************************************************************************/
/*!
\brief
Code to drop an item
\param itemname - name of item
*/
/****************************************************************************/
void  CCharacter::dropItem(string itemname)
{
	{
		for (int i = 0; i < returnInvenSize(); ++i)
		{
			if ( getInventory(i)->getItemName() == itemname)
			{
				inven_list.erase(inven_list.begin()+i);
				break;
			}
		}
	}
}

string CCharacter::getItemName(int n)
{
	return getInventory(n)->getItemName();
}
/****************************************************************************/
/*!
\brief
Erases the player's inventory
*/
/****************************************************************************/
void  CCharacter::sellItems(int n)
{
	inven_list.erase(inven_list.begin()+n);
}
/****************************************************************************/
/*!
\brief
Gets player character inventory
*/
/****************************************************************************/
CItem* CCharacter::getInventory(int n)
{
	return this->inven_list[n];
}

int CCharacter::returnInvenSize()
{
	return inven_list.size();
}

void CCharacter::setInventoryCap(int size)
{
	inventoryCap = size;
}

int CCharacter::getInventoryCap(void)
{
	return inventoryCap;
}