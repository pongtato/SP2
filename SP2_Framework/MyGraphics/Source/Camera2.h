/****************************************************************************/
/*!
\file Camera2.h
\Author SP2 Group 4
\par
\brief
Codes to define 3D Space
*/
/****************************************************************************/
#ifndef CAMERA_2_H
#define CAMERA_2_H

#include "Camera.h"

class Camera2 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera2();
	~Camera2();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
};

#endif