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
	if(position.x <= - 55 && position.x >= -85 && position.z <= 70 &&  position.z >= 25)
	{
		EntranceDoor = true;
	}
	else
	{
		EntranceDoor = false;
	}
}

void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 65.f;
if(Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(5 * CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;
	}
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(5 * -CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		up = rotation * up;
		target = view + position;
	}
	if(Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(5 * CAMERA_SPEED * dt);
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
	if(Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(5 * -CAMERA_SPEED * dt);
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
		position -= right * CAMERA_SPEED * dt;
		target -= right * CAMERA_SPEED * dt;
	}
	if(Application::IsKeyPressed('D') && Limit(position,target, 450, CAMERA_SPEED))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * CAMERA_SPEED * dt;
		target += right * CAMERA_SPEED * dt;			
	}
	if(Application::IsKeyPressed('W') && Limit(position,target, 450, CAMERA_SPEED))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position += view * CAMERA_SPEED * dt;
		target += view * CAMERA_SPEED * dt;
	}
	if(Application::IsKeyPressed('S') && Limit(position,target, 450, CAMERA_SPEED))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position -= view * CAMERA_SPEED * dt;
		target -= view * CAMERA_SPEED * dt;
	}

	Doorsensor(position,CAMERA_SPEED);
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}