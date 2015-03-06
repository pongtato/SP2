/****************************************************************************/
/*!
\file CameraSecurity.h
\Author SP2 Group 4
\par
\brief
Main Header file for CameraSecurity
*/
/****************************************************************************/
#pragma once

#include <Windows.h>
#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

using namespace std;

class CCameraSecurity : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	CCameraSecurity(void);
	~CCameraSecurity(void);

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	Application test;
	float Limit;
};

