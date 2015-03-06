#pragma once
#include <irrKlang.h>

#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;

class Csounds
{
private:
	ISoundEngine* engine;
public:
	Csounds(void);
	~Csounds(void);

	bool MainMusic();
	bool Explosion();
	bool Bullet();
	bool MetalClank();
	bool CheckOut();
	bool Reload();
};

