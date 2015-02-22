#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
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
	static const float CAMERA_SPEED = 10.f;
	double MouseX = 0;
	double MouseY = 0;
	double OldMousePosX = MouseX;
	double OldMousePosY = MouseY;
	test.mousePos();
	MouseX = test.getPosX();
	MouseY = test.getPosY();
	


	if(Application::IsKeyPressed(VK_SHIFT) && player.getStamina()!=0)
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

	if((MouseX - OldMousePosX) < 400)
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(3*CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;
	}

	if((MouseX - OldMousePosX) < 398)
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(24 * CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;
	}
	if((MouseX - OldMousePosX) > 400)
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(3*-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;
	}

	if((MouseX - OldMousePosX) > 402)
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(24 * -CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;
	}
	if((MouseY - OldMousePosY) < 299.5)
	{
		float pitch = (float)(3*CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = view + position;
	}
	if((MouseY - OldMousePosY) < 297)
	{
		float pitch = (float)(12 *CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = view + position;
	}
	if((MouseY - OldMousePosY) > 300)
	{
		float pitch = (float)(3*-CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = view + position;
	}
	if((MouseY - OldMousePosY) > 302)
	{
		float pitch = (float)(12 * -CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = view + position;
	}
	if(Application::IsKeyPressed('A') && Limit(position,target, 450, CAMERA_SPEED))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * sprint *CAMERA_SPEED * dt;
		target -= right * sprint *CAMERA_SPEED * dt;
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
	}
	if(Application::IsKeyPressed('W') && Limit(position,target, 450, CAMERA_SPEED))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position += view * sprint *CAMERA_SPEED * dt;
		target += view * sprint *CAMERA_SPEED * dt;
	}
	if(Application::IsKeyPressed('S') && Limit(position,target, 450, CAMERA_SPEED))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position -= view * sprint *CAMERA_SPEED * dt;
		target -= view * sprint *CAMERA_SPEED * dt;
	}

	Doorsensor(position,CAMERA_SPEED);
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}