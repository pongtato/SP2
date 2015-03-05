#include "Csounds.h"


Csounds::Csounds(void)
{
	engine = createIrrKlangDevice();
}


Csounds::~Csounds(void)
{
	engine->drop();
}

bool Csounds::MainMusic()
{
	ISound* music = engine->play3D("../irrKlang/media/ophelia.mp3", vec3df(0,0,0), true, false, true);
	return true;
}

bool Csounds::Explosion()
{
	ISound* music = engine->play3D("../irrKlang/media/Bomb-Sound.mp3", vec3df(0,0,0), false, false, true);
	return true;
}

bool Csounds::Bullet()
{
	ISound* music = engine->play3D("../irrKlang/media/BulletShot.mp3", vec3df(0,0,0), false, false, true);
	return true;
}

bool Csounds::MetalClank()
{
	ISound* music = engine->play3D("../irrKlang/media/Metal Clank.mp3", vec3df(0,0,0), false, false, true);
	return true;
}