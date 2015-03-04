
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include <string>


class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsKeyReleased(unsigned short key);

	void setPosX(double x);
	void setPosY(double y);
	double getPosX(void);
	double getPosY(void);
	double DMX;
	double DMY;
	bool restart;

	void mousePos(void);

private:

	//Declare a window object
	StopWatch m_timer;
	double MX;
	double MY;
};

#endif