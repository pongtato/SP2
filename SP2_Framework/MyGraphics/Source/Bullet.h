#pragma once
#include <iostream>
#include "Vector3.h"
#include <math.h>
#include "MathsConversion.h"

using namespace std;

class CBullet
{
public:	
	Vector3 Position;
	Vector3 Direction;
	Vector3 Velocity;
	Vector3 VelocityPolice;
	Vector3 Range;

	bool alive;
	int bullet;
	int clip;

	CBullet(void);
	~CBullet(void);

	bool BulletUpdate(double dt);
	bool BulletUpdatePolice(double dt);
	void BulletRender(void);
	float ShootInterval;
	float RTime;
};

