/****************************************************************************/
/*!
\file Camera.cpp
\Author SP2 Group 4
\pa
\brief
Codes to move and initiate a camera.
*/
/****************************************************************************/
#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"
/****************************************************************************/
/*!
\brief
Default constructor
*/
/****************************************************************************/
Camera::Camera()
{
	Reset();
}
/****************************************************************************/
/*!
\brief
Default destructor
*/
/****************************************************************************/
Camera::~Camera()
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
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}
/****************************************************************************/
/*!
\brief
Resets vectors to the base starting set.
*/
/****************************************************************************/
void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}
/****************************************************************************/
/*!
\brief
Codes for camera controls, and sets the speed for camera movement.
*/
/****************************************************************************/
void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;
	if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
	{
		position.x -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
	{
		position.x += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
	{
		position.y += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
	{
		position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
}