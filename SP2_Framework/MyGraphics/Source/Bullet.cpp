/****************************************************************************/
/*!
\file Bullet.cpp
\Author SP2 Group 4
\par
\brief
Codes for bullet physics and collisions.
*/
/****************************************************************************/
#include "Bullet.h"


CBullet::CBullet(void)
: Position		( Vector3() )
, Direction		( Vector3() )
, Velocity		( Vector3( 5.0f, 5.0f, 5.0f ) )
, VelocityPolice( Vector3( 3.0f, 3.0f, 3.0f ) )
, Zoom			( Vector3( 5.0f, 5.0f, 5.0f ) )
, Range			( Vector3( 500.0f, 500.0f, 500.0f ) )
, ShootInterval (0)
, alive			( true )
,RTime			(0)
{
	bullet = 30;
	clip = 3;
}

CBullet::~CBullet(void)
{
}

bool CBullet::BulletUpdate(double dt)
{
	if ( !alive ) return false;

	// Normalise Direction
	Direction.Normalize();
	Position += Vector3( Direction.x * Velocity.x , Direction.y * Velocity.y , Direction.z * Velocity.z );

	if ( Position.x > Range.x || Position.x < -Range.x || Position.y > Range.y || Position.y < -Range.y || Position.z > Range.z || Position.z < -Range.z )
		alive = false;
	return true;
}

bool CBullet::BulletUpdatePolice(double dt)
{
	if ( !alive ) return false;

	// Normalise Direction
	Direction.Normalize();
	Position += Vector3( Direction.x * VelocityPolice.x , Direction.y * VelocityPolice.y , Direction.z * VelocityPolice.z );

	if ( Position.x > Range.x || Position.x < -Range.x || Position.y > Range.y || Position.y < -Range.y || Position.z > Range.z || Position.z < -Range.z )
		alive = false;
	return true;
}

bool CBullet::DummyZoom(double dt)
{
	if ( !alive ) return false;

	// Normalise Direction
	Direction.Normalize();
	Position = Vector3( Direction.x + VelocityPolice.x , Direction.y + VelocityPolice.y , Direction.z + VelocityPolice.z );

	if ( Position.x > Range.x || Position.x < -Range.x || Position.y > Range.y || Position.y < -Range.y || Position.z > Range.z || Position.z < -Range.z )
		alive = false;
	return true;
}
