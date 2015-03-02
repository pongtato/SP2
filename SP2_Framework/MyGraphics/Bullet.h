#pragma once
#include <iostream>
#include "Vector3.h"
#include <math.h>
#include "MyMath.h"

using namespace std;

class CBullet
{
private:
	Vector3 vec;
	float BulletSpeed;
public:
	CBullet(void);
	~CBullet(void);

	void SetBulletPos(float X, float Y, float Z);
	Vector3 getBulletPos(void);
	void SetSpeed(float s);
	float GetSpeed(void);
	Vector3 GetDirection(void);
	Vector3 Normalise(void);
	float getMagnitude(float X, float Y, float Z);
};

