#pragma once
#include "Model.h"
#include "ModelLoaderClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureClass.h"

#include "Camera.h"

#include <cstring>
class BC_Model
{
public:
	BC_Model();
	~BC_Model();

	Model model;
	ModelImport modelLoader;

	Texture texture;

	int lightColLocation;
	int lightPositionLocation;

	int importModelLocation;
	int importViewLocation;
	int importProjectionLocation;

	int normalMatrixLocation;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 lightColour;
	glm::vec3 lightPosition;

	glm::mat4 modelRotate;
	glm::mat4 modelScale;
	glm::mat4 modelTranslate;

	glm::mat4 normalMatrix;

	void init(int w, int h, std::string modelPath, std::string texturePath);
	void update(GLuint elapsedTime, Camera cam);
	void render();
};

