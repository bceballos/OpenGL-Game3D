#pragma once

#include "BC_Sphere.h"
#include "BC_Player.h"
#include "Camera.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

class BC_CircleManager
{
public:
	BC_CircleManager();
	~BC_CircleManager();

	void init(int amount, int w, int h);
	void update(float bX_r, float bX_l, float bY_t, float bY_b, BC_Sphere &player, Camera cam, GLuint elapsedTime);
	void render();
	void repopulate();

	int w, h;
	int initAmount;
	int amount;

	std::vector<BC_Sphere> circles;
};

