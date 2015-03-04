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
	Vector3 PPosition;
	Vector3 Direction;
	Vector3 Velocity;
	Vector3 VelocityPolice;
	Vector3 TrolleyOffset;
	Vector3 Range;
	Vector3 RangeTrolley;

	bool alive;
	int bullet;
	int clip;

	CBullet(void);
	~CBullet(void);

	bool BulletUpdate(double dt);
	bool BulletUpdatePolice(double dt);
	bool TrolleyLimiter(float x,float y, float z, double dt);
	void BulletRender(void);
	float ShootInterval;
	float RTime;
};

