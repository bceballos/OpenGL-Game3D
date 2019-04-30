#pragma once
#include "BC_Sphere.h"
#include "BC_Player.h"
#include "BC_Bullet.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
class BC_SphereManager
{
public:
	BC_SphereManager();
	~BC_SphereManager();

	std::vector<BC_Sphere> bubbles;

	int amount, initAmount;
	int w, h;
	int lightingModel;

	void init(int amount, int w, int h, int lightingModel);
	void update(GLuint elapsedTime, Camera cam, float bX_r, float bX_l, float bY_t, float bY_b, BC_Player &player, std::vector<BC_Bullet> &bullets);
	void repopulate();
};

