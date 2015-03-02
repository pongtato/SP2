#include "Bullet.h"


CBullet::CBullet(void)
{
	vec.x = 0;
	vec.y = 0;
	vec.z = 0;
	BulletSpeed = 0;
}


CBullet::~CBullet(void)
{
}

void CBullet::SetBulletPos(float X,float Y,float Z)
{
	vec.x = X;
	vec.y = Y;
	vec.z = Z;
}

Vector3 CBullet::getBulletPos(void)
{
	return vec;
}

void CBullet::SetSpeed(float s)
{
	BulletSpeed = s;
}

float CBullet::GetSpeed(void)
{
	return BulletSpeed;
}

float CBullet::getMagnitude(float X, float Y, float Z)
{
	return sqrtf(X * X + Y * Y + Z * Z);
}

Vector3 CBullet::GetDirection(void)
{
	float magnitude = getMagnitude();

	if (magnitude == 0)
	{
		const float inv = 1 / magnitude;
		const float x = (float)( acos(this->vec.x * inv) * RADTODEG );
		const float y = (float)( acos(this->vec.y * inv) * RADTODEG );
	}

	return Vector3( x, y, z );
}