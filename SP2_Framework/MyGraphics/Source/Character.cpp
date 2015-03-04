#include "Character.h"


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


CCharacter::~CCharacter(void)
{
}

void CCharacter::setHealth(int hp)
{
	health -= hp;
}

void CCharacter::setMoney(double cash)
{
	money -= cash;
}

void CCharacter::setBullets(int ammo)
{
	bullets = ammo;
}

void CCharacter::setStamina(int stam)
{
	stamina -= stam;
}

void CCharacter::setDetection(float detect)
{
	detection = detect;
}

void CCharacter::setClass(string job)
{
	career = job;
}

double CCharacter::getMoney(void)
{
	return money;
}

int CCharacter::getHealth(void)
{
	return health;
}

int CCharacter::getStamina(void)
{
	return stamina;
}

float CCharacter::getDetection(void)
{
	return detection;
}

string CCharacter::getClass(void)
{
	return career;
}

void  CCharacter::setInventory(string name, double price)
{
	CItem * inven = new CItem();
	inven->setItemName(name);
	inven->setItemPrice(price);
	inven_list.push_back(inven);
}

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

void  CCharacter::sellItems(int n)
{
	inven_list.erase(inven_list.begin()+n);
}

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