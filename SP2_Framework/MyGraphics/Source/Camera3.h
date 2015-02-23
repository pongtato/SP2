#ifndef CAMERA_3_H
#define CAMERA_3_H

#include <Windows.h>
#include "Camera.h"
#include "Application.h"
#include "Character.h"
#include <string>

using namespace std;

class Camera3 : public Camera
{
private:
	int staminaDupe;
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	CCharacter player;

	Camera3();
	~Camera3();

	bool EntranceDoor;
	bool ExitDoor;
	bool moving;

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	bool Limit(Vector3& pos, Vector3& target, int Border, float camSpeed);
	void Doorsensor(Vector3& pos, float camSpeed);
	virtual void Reset();
	void setStaminaDupe(int stam);
	int getStaminaDupe(void);
	Application test;
};

#endif