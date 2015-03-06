/****************************************************************************/
/*!
\file Camera3.cpp
\Author SP2 Group 4
\par
\brief
Codes to move and initiate a camera.
*/
/****************************************************************************/
#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
/****************************************************************************/
/*!
\brief
Default constructor
*/
/****************************************************************************/
Camera3::Camera3()
{
	cameraState = 0;
	CameraLock = 0;
	CameraMode = false;
	CamRotation = 0;
	CAMERA_SPEED = 10.f;
	TrolleyMode = false;
	RepeatPress = true;
	State = 1;
	//isCollide = false;
	downSight = false;
}
/****************************************************************************/
/*!
\brief
Default Destructor
*/
/****************************************************************************/
Camera3::~Camera3()
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
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}
/***************************************************************/
/*!
/brief
Limitation codes for the camera.

/param position - position vector
/param target - point target vector
/param Border - border coordinates to bind the camera
/param camSpeed - The speed of the camera movement
*/
/***************************************************************/
bool Camera3::Limit( Vector3& position,Vector3& target, int Border, float camSpeed)
{
	camSpeed *= 0.02;

	if(position.x < -Border)
	{
		position.x += camSpeed;
		target.x += camSpeed;
	}

	if(position.x > Border)
	{
		position.x -= camSpeed;
		target.x -= camSpeed;
	}

	if(position.y < -Border)
	{
		position.y += camSpeed;
		target.y += camSpeed;
	}

	if(position.y > Border)
	{
		position.y -= camSpeed;
		target.y -= camSpeed;
	}

	if(position.z < -Border)
	{
		position.z += camSpeed;
		target.z += camSpeed;
	}

	if(position.z > Border)
	{
		position.z -= camSpeed;
		target.z -= camSpeed;
	}
	else
	{
		return true;
	}
}
/***************************************************************/
/*!
/brief
Codes to open the staff shutter door.

/param pos - position vector
/param camSpeed - The speed of the camera movement
*/
/***************************************************************/
void Camera3::StaffDoorsensor(Vector3& pos, float camSpeed)
{
	camSpeed *= 0.05;
	//Right door
	if(position.x <= 65 && position.x >= 55 && position.z <= -25 &&  position.z >= -45)
	{
		RightStaffDoor = true;
	}
	else
	{
		RightStaffDoor = false;
	}
	//Left door
	if(position.x >=  -76 && position.x <= -65 && position.z <= -25 &&  position.z >= -45)
	{
		LeftStaffDoor = true;
	}
	else
	{
		LeftStaffDoor = false;
	}
}
/***************************************************************/
/*!
/brief
Codes to open the back shutter door.

/param pos - position vector
/param camSpeed - The speed of the camera movement
*/
/***************************************************************/
void Camera3::Shuttersensor(Vector3& pos, float camSpeed)
{
	camSpeed *= 0.05;
	if(position.x <= 10 && position.x >= -20 && position.z <= -55 &&  position.z >= -70)
	{
		ShutterDoor = true;
	}
	else
	{
		ShutterDoor = false;
	}
}
/***************************************************************/
/*!
/brief
Codes to open the mart shutter door.

/param pos - position vector
/param camSpeed - The speed of the camera movement
*/
/***************************************************************/
void Camera3::Doorsensor(Vector3& pos, float camSpeed)
{
	camSpeed *= 0.05;
	if(position.x <= - 50 && position.x >= -75 && position.z <= 70 &&  position.z >= 25)
	{
		EntranceDoor = true;
	}
	else
	{
		EntranceDoor = false;
	}

	if(position.x >=  50 && position.x <= 75 && position.z <= 70 &&  position.z >= 25)
	{
		ExitDoor = true;
	}
	else
	{
		ExitDoor = false;
	}
}
/***************************************************************/
/*!
/brief
Codes to set the stamina dupe.
*/
/***************************************************************/
void Camera3::setStaminaDupe(int stam)
{
	staminaDupe = stam;
}
/***************************************************************/
/*!
/brief
Codes to get the stamina dupe.
*/
/***************************************************************/
int Camera3::getStaminaDupe(void)
{
	return staminaDupe;
}
/****************************************************************************/
/*!
\brief
Codes for camera controls, trolley controls, and 
*/
/****************************************************************************/
void Camera3::Update(double dt)
{	
	int sprint = 1;
	//bool moving = false;
	test.mousePos();
	MouseX = test.getPosX();
	MouseY = test.getPosY();

	if(Application::IsKeyPressed(VK_SHIFT) && player.getStamina()!=0 && moving == true)
	{
		player.setStamina(1);
		sprint = 5;
	}
	else if (Application::IsKeyReleased(VK_SHIFT) && player.getStamina() != 200)
	{
		player.setStamina(-1);
		sprint = 1;
	}

	setStaminaDupe(player.getStamina());

	if (getCameraState() == 1 )
	{
		CAMERA_SPEED = 5.f;
	}

	if ( isCollide == false)
	{
		if(MouseX < 960)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(CAMERA_SPEED * dt);
			if ( isCollide == false)
				CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}

		if(MouseX < 958  && downSight == false)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(18*CAMERA_SPEED * dt);
			if ( isCollide == false)
				CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}
		if(MouseX > 960)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(-CAMERA_SPEED * dt);
			if ( isCollide == false)
				CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}

		if(MouseX > 962 && downSight == false)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(18*-CAMERA_SPEED * dt);
			if ( isCollide == false)
				CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}
		if (CameraLock < 40)
		{
			if (getCameraState() == 0 )
			{
				if(MouseY < 540 && MouseY >=538)
				{
					float pitch = (float)(CAMERA_SPEED * dt);
					if ( isCollide == false)
						CamRotationY+=pitch;
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);
					right.y = 0;
					right.Normalize();
					up = right.Cross(view).Normalized();
					Mtx44 rotation;
					rotation.SetToRotation(pitch, right.x, right.y, right.z);
					view = rotation * view;
					target = view + position;
					CameraLock+=pitch; 
				}
				if(MouseY < 538 && downSight == false)
				{
					float pitch = (float)(12*CAMERA_SPEED * dt);
					if ( isCollide == false)
						CamRotationY+=pitch;
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);
					right.y = 0;
					right.Normalize();
					up = right.Cross(view).Normalized();
					Mtx44 rotation;
					rotation.SetToRotation(pitch, right.x, right.y, right.z);
					view = rotation * view;
					target = view + position;	
					CameraLock+=pitch;
				}
			}
		}
		if (CameraLock > -40)
		{
			if (getCameraState() == 0 )
			{
				if(MouseY > 540 && MouseY <= 542 )
				{
					float pitch = (float)(-CAMERA_SPEED * dt);
					if ( isCollide == false)
						CamRotationY+=pitch;
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);
					right.y = 0;
					right.Normalize();
					up = right.Cross(view).Normalized();
					Mtx44 rotation;
					rotation.SetToRotation(pitch, right.x, right.y, right.z);
					view = rotation * view;
					target = view + position;
					CameraLock+=pitch;
				}
				if(MouseY > 542  && downSight == false)
				{
					float pitch = (float)(12*-CAMERA_SPEED * dt);
					if ( isCollide == false)
						CamRotationY+=pitch;
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);
					right.y = 0;
					right.Normalize();
					up = right.Cross(view).Normalized();
					Mtx44 rotation;
					rotation.SetToRotation(pitch, right.x, right.y, right.z);
					view = rotation * view;
					target = view + position;
					CameraLock+=pitch;
				}
			}
		}
	}

	if ( isCollide != true)
	{
		PrevCamRotationX = CamRotationX;
		PrevCamRotationY = CamRotationY;

	}
	
	if ( CameraMode == false)
		{

			if(Application::IsKeyPressed('A') && Limit(position,target, 450, CAMERA_SPEED))
			{
				Vector3 view = (target - position).Normalized();
				view.y = 0;
				Vector3 right = view.Cross(up);
				right.y = 0;
				right.Normalize();
				position -= right * sprint *CAMERA_SPEED * dt;
				target -= right * sprint *CAMERA_SPEED * dt;
				moving = true;
			}
			if(Application::IsKeyPressed('D') && Limit(position,target, 450, CAMERA_SPEED))
			{
				Vector3 view = (target - position).Normalized();
				view.y = 0;
				Vector3 right = view.Cross(up);
				right.y = 0;
				right.Normalize();
				position += right * sprint *CAMERA_SPEED * dt;
				target += right * sprint *CAMERA_SPEED * dt;	
				moving = true;
			}
			if(Application::IsKeyPressed('W') && Limit(position,target, 450, CAMERA_SPEED))
			{
				Vector3 view = (target - position).Normalized();
				view.y = 0;
				position += view * sprint *CAMERA_SPEED * dt;
				target += view * sprint *CAMERA_SPEED * dt;
				moving = true;
			}
			if(Application::IsKeyPressed('S') && Limit(position,target, 450, CAMERA_SPEED))
			{
				Vector3 view = (target - position).Normalized();
				view.y = 0;
				position -= view * sprint *CAMERA_SPEED * dt;
				target -= view * sprint *CAMERA_SPEED * dt;
				moving = true;
			}
			if(Application::IsKeyReleased('S') && Application::IsKeyReleased('W') && Application::IsKeyReleased('A') && Application::IsKeyReleased('D'))
			{
				moving = false;
			}
		}
	Doorsensor(position,CAMERA_SPEED);
	Shuttersensor(position, CAMERA_SPEED);
	StaffDoorsensor(position, CAMERA_SPEED);
}
/****************************************************************************/
/*!
\brief
Resets vectors to the base starting set.
*/
/****************************************************************************/
void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
/****************************************************************************/
/*!
\brief
Sets the camera state
*/
/****************************************************************************/
void Camera3::setCameraState(int state)
{
	cameraState = state;
}
/****************************************************************************/
/*!
\brief
Gets the camera state
*/
/****************************************************************************/
int Camera3::getCameraState(void)
{
	return cameraState;
}