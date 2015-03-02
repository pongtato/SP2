#include "Bullet.h"


CBullet::CBullet(void)
: Position	( Vector3() )
, Direction	( Vector3() )
, Velocity	( Vector3( 5.0f, 5.0f, 5.0f ) )
, Range		( Vector3( 500.0f, 500.0f, 500.0f ) )
, alive		( true )
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

void CBullet::BulletRender(void)
{
}