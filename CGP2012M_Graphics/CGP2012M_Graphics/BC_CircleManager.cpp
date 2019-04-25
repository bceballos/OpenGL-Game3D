#include "BC_CircleManager.h"



BC_CircleManager::BC_CircleManager()
{
}


BC_CircleManager::~BC_CircleManager()
{
}

void BC_CircleManager::init(int amount, int w, int h)
{
	srand(time(0));

	this->amount = amount;
	initAmount = amount;
	for (int i = 0; i < amount; i++)
	{
		float randValue = rand() % 6 + 2;
		float randValue1 = rand() % 5 + 2;

		circles.push_back(BC_Sphere());
		circles.back().init(w, h, "..//..//Assets//Models//blenderSphere.obj", "..//..//Assets//Textures//deathstar.png", randValue, randValue1);
	}

	this->w = w;
	this->h = h;
}

void BC_CircleManager::update(float bX_r, float bX_l, float bY_t, float bY_b, BC_Sphere &player, Camera cam, GLuint elapsedTime)
{
	float bEuclidianDistance;

	for (int i = 0; i < amount; i++)
	{

		float pEuclidianDistance = std::sqrt(pow((circles[i].position.x - player.position.x), 2.0f) + pow((circles[i].position.y - player.position.y), 2.0f) + pow((circles[i].position.y - player.position.z), 2.0f));
		//if (player.shot) {
		//	bEuclidianDistance = std::sqrt(pow((circles[i].position.x - player.bullet[0].position.x), 2.0f) + pow((circles[i].position.y - player.bullet[0].position.y), 2.0f) + pow((circles[i].position.z - player.bullet[0].position.z), 2.0f));
		//}


		if (pEuclidianDistance <= 0.7) {
			circles[i].destroyed = true;
			this->amount--;
		}
		else {
			circles[i].update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b);
		}

		//if (player.shot && bEuclidianDistance <= 0.6) {
		//	player.shot = false;
		//	player.bullet.erase(player.bullet.begin());
		//	circles[i].destroyed = true;
		//	//this->amount--;
		//}

		//if (circles[i].animationCalls == 15) {
		//	circles.erase(circles.begin() + i);
		//	this->amount--;
		//}
	}
	repopulate();
}

void BC_CircleManager::render()
{
	for (int i = 0; i < amount; i++)
	{
		circles[i].render();
	}
}

void BC_CircleManager::repopulate()
{
	srand(time(0));
	if (amount < initAmount) {
		for (int i = 0; i < (initAmount - amount); i++) {
			float randValue = rand() % 15 + 1;
			float randValue1 = rand() % 8 + 1;

			circles.push_back(BC_Sphere());
			circles.back().init(w, h, "..//..//Assets//Models//blenderSphere.obj", "..//..//Assets//Textures//deathstar.png", randValue, randValue1);
			amount++;
		}
	}
}
