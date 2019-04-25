#include "BC_SphereManager.h"



BC_SphereManager::BC_SphereManager()
{
}


BC_SphereManager::~BC_SphereManager()
{
}

void BC_SphereManager::init(int amount, int w, int h)
{
	this->amount = amount;
	this->w = w;
	this->h = h;

	for (int i = 0; i < amount; i++) {
		bubbles.push_back(BC_Sphere());
		bubbles.back().init(w, h, "..//..//Assets//Models//blenderSphere.obj", "..//..//Assets//Textures//deathstar.png", 1.0f * i, 1.0f * i);
	}
}

void BC_SphereManager::update(GLuint elapsedTime, Camera cam, float bX_r, float bX_l, float bY_t, float bY_b)
{
	for (int i = 0; i < amount; i++) {
		bubbles[i].update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b);
		bubbles[i].render();
	}
}