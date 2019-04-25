#pragma once
#include "BC_Sphere.h"
#include <vector>
class BC_SphereManager
{
public:
	BC_SphereManager();
	~BC_SphereManager();

	std::vector<BC_Sphere> bubbles;

	int amount;
	int w, h;

	void init(int amount, int w, int h);
	void update(GLuint elapsedTime, Camera cam, float bX_r, float bX_l, float bY_t, float bY_b);
};
