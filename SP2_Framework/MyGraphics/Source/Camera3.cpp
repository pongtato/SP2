#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

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
	isCollide = false;
	CamRotationX = 0;
	CamRotationY = 0;
}

Camera3::~Camera3()
{
}

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

void Camera3::setStaminaDupe(int stam)
{
	staminaDupe = stam;
}

int Camera3::getStaminaDupe(void)
{
	return staminaDupe;
}

void Camera3::Update(double dt)
{	
	int sprint = 1;
	//bool moving = false;
	double MouseX = 0;
	double MouseY = 0;
	double OldMousePosX = MouseX;
	double OldMousePosY = MouseY;
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
		if((MouseX - OldMousePosX) < 400)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(9*CAMERA_SPEED * dt);
			CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}
		if((MouseX - OldMousePosX) < 398)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(18 * CAMERA_SPEED * dt);
			CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}
		if((MouseX - OldMousePosX) > 400)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(9*-CAMERA_SPEED * dt);
			CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}
		if((MouseX - OldMousePosX) > 402)
		{
			Vector3 view = (target - position).Normalized();
			float yaw = (float)(18 * -CAMERA_SPEED * dt);
			CamRotationX+=yaw;
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view = rotation * view;
			up = rotation * up;
			target = view + position;
		}
		if (CameraLock < 20)
		{
			if (getCameraState() == 0 )
			{
				if((MouseY - OldMousePosY) < 299.5)
				{
					float pitch = (float)(12*CAMERA_SPEED * dt);
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
					CameraLock++;	
				}
			}
		}
	if (CameraLock > -15)
	{
		if (getCameraState() == 0 )
		{
			if((MouseY - OldMousePosY) > 300)
			{
				float pitch = (float)(12*-CAMERA_SPEED * dt);
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
				CameraLock--;
			}
		}
	}
}
	
	


	if ( CameraMode == false && TrolleyMode == false)
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

	if  ( TrolleyMode == true)
	{
		if (position.z <= 23 || position.z >= 23 && CamRotation != 180)
		{
			if(Application::IsKeyPressed('A')&&(RepeatPress == true))
			{
				if ( State == 1)
				{
					State = 2;
				}
				else if (State == 2)
				{
					State = 4;
				}
				else if (State ==4)
				{
					State = 3;
				}
				else if (State == 3)
				{
					State = 1;
				}
				fixCamera = true;
				TrolleyStateR = CamRotation+90;
				RepeatPress = false;
			}
		}

		if (position.z <= 23 || position.z >= 23 && CamRotation != 0)
		{
			if(Application::IsKeyPressed('D')&&(RepeatPress == true))
			{
				if ( State == 1)
				{
					State = 3;
				}
				else if (State == 2)
				{
					State = 1;
				}
				else if (State ==4)
				{
					State = 2;
				}
				else if (State == 3)
				{
					State = 4;
				}
				fixCamera = true;
				TrolleyStateR = CamRotation-90;
				RepeatPress = false;
			}
		}

		if(Application::IsKeyPressed('W')&&(RepeatPress == true))
		{
				if (State == 1)
				{
					if (position.x <= -69)
					{
						TrolleyStateT = -35;
					}
					else if (position.x > -37 && position.x < -33 )
					{
						TrolleyStateT = 0;
					}
					else if (position.x > -2 && position.x < 2 )
					{
						TrolleyStateT = 35;
					}
				}
				else if (State == 2)
				{
					if (position.z >= 23)
					{
						TrolleyStateT = 8;
					}
					else if (position.z < 10 && position.z > 6 )
					{
						TrolleyStateT = -7;
					}
					else if (position.z > -9 && position.z < -6 )
					{
						TrolleyStateT = -25;
					}
				}
				else if (State == 3)
				{
					if (position.z <= -23)
					{
						TrolleyStateT = -8;
					}
					else if (position.z < 10 && position.z > 6 )
					{
						TrolleyStateT = 25;
					}
					else if (position.z > -9 && position.z < -6 )
					{
						TrolleyStateT = 7;
					}
				}
				else if (State == 4)
				{
					if (position.x >= 35)
					{
						TrolleyStateT = 0;
					}
					else if (position.x > -37 && position.x < -33 )
					{
						TrolleyStateT = -69;
					}
					else if (position.x > -2 && position.x < 2 )
					{
						TrolleyStateT = -34;
					}
				}
				fixTranslation = true;
				RepeatPress = false;
			}
	}

	if(fixCamera == true)
	{
		if(CamRotation > TrolleyStateR)
		{
			CamRotation-= 3;
		}
		else 
			if(CamRotation < TrolleyStateR)
			{
				CamRotation+= 3;
			}
			else
			{
				RepeatPress = true;
				fixCamera = false;
			}
	}
	else if (fixTranslation == true)
	{
		if (State == 1)
		{
			if (position.x < TrolleyStateT)
			{
				position.x++;
				target.x++;
			}
			else
			{
				RepeatPress = true;
				fixTranslation = false;
			}
		}
		else if (State == 2)
		{
			if (position.z > TrolleyStateT)
			{
				position.z--;
				target.z--;
			}
			else
			{
				RepeatPress = true;
				fixTranslation = false;
			}
		}
		else if (State == 3)
		{
			if (position.z < TrolleyStateT)
			{
				position.z++;
				target.z++;
			}
			else
			{
				RepeatPress = true;
				fixTranslation = false;
			}
		}
		else if (State == 4)
		{
			if (position.x > TrolleyStateT)
			{
				position.x--;
				target.x--;
			}
			else
			{
				RepeatPress = true;
				fixTranslation = false;
			}
		}
	}
	Doorsensor(position,CAMERA_SPEED);
	Shuttersensor(position, CAMERA_SPEED);
	StaffDoorsensor(position, CAMERA_SPEED);
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void Camera3::setCameraState(int state)
{
	cameraState = state;
}

int Camera3::getCameraState(void)
{
	return cameraState;
}