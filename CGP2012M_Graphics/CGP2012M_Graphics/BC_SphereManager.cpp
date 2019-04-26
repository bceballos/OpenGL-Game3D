#include "BC_SphereManager.h"



BC_SphereManager::BC_SphereManager()
{
}


BC_SphereManager::~BC_SphereManager()
{
}

void BC_SphereManager::init(int amount, int w, int h, int lightingModel)
{
	srand(time(0));

	this->amount = amount;
	initAmount = amount;
	this->w = w;
	this->h = h;
	this->lightingModel = lightingModel;

	for (int i = 0; i < amount; i++) {


		bubbles.push_back(BC_Sphere());
		bubbles.back().init(w, h, "..//..//Assets//Models//blenderSphere.obj", "..//..//Assets//Textures//earthmap1k.png", (rand() % 5 + 1), (rand() % 4 + 1), lightingModel);
	}
}

void BC_SphereManager::update(GLuint elapsedTime, Camera cam, float bX_r, float bX_l, float bY_t, float bY_b, BC_Player &player)
{
	for (int i = 0; i < amount; i++) {
		float pEuclidianDistance = std::sqrt(pow((bubbles[i].position.x - player.position.x), 2.0f) + pow((bubbles[i].position.y - player.position.y), 2.0f) + pow((bubbles[i].position.z - player.position.z), 2.0f));

		if (pEuclidianDistance <= 0.9) {
			if (!bubbles[i].destroyed && !player.immortal) {
				player.health--;
				player.immortal = true;
				player.triggered = true;
				bubbles.erase(bubbles.begin() + i);
				this->amount--;
			}
			else {
				bubbles[i].update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b);
				bubbles[i].render();
			}
		}
		else {
			bubbles[i].update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b);
			bubbles[i].render();
		}
	}

	repopulate();
}

void BC_SphereManager::repopulate()
{
	srand(time(0));
	if (amount < initAmount) {
		for (int i = 0; i < (initAmount - amount); i++) {
			bubbles.push_back(BC_Sphere());
			bubbles.back().init(w, h, "..//..//Assets//Models//blenderSphere.obj", "..//..//Assets//Textures//earthmap1k.png", (rand() % 4), (rand() % 3), lightingModel);
			amount++;
		}
	}
}