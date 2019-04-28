#pragma once
#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "ShaderClass.h"
#include "ModelLoaderClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
//#include "SDL.h"
#include <vector>
#include <cstring>

class BC_Skybox
{
public:
	BC_Skybox();
	~BC_Skybox();

	void init();
	void update();
	void render();
};

