/****************************************************************************/
/*!
\file CameraSecurity.cpp
\Author SP2 Group 4
\par
\brief
Codes to update the Security Cameras.
*/
/****************************************************************************/
#include "CameraSecurity.h"

/****************************************************************************/
/*!
\brief
Default constructor
*/
/****************************************************************************/
CCameraSecurity::CCameraSecurity(void)
{
	Limit = 45;
}

/****************************************************************************/
/*!
\brief
Default destructor
*/
/****************************************************************************/
CCameraSecurity::~CCameraSecurity(void)
{
}

/***************************************************************/
/*!
/brief
Initiation codes for the position, target, and positive z-coordinates.

/param pos - position vector
/param target - point target vector
/param up - positive z-coordinates vector
*/
/***************************************************************/
void CCameraSecurity::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}
/****************************************************************************/
/*!
\brief
Codes for camera controls, trolley controls, and 
*/
/****************************************************************************/
void CCameraSecurity::Update(double dt)
{
	double MouseX = 0;
	double MouseY = 0;
	double OldMousePosX = MouseX;
	double OldMousePosY = MouseY;
	test.mousePos();
	MouseX = test.getPosX();
	MouseY = test.getPosY();
	float CAMERA_SPEED = 10;

	if ( Limit > -20 )
	{
		if((MouseX - OldMousePosX) < 960)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(4*CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
			Limit--;
		}
	}
	if ( Limit < 60)
	{
		if((MouseX - OldMousePosX) > 960)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(4*-CAMERA_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
			Limit++;
		}
	}
}