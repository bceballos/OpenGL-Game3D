#pragma once
#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "ShaderClass.h"
#include "ModelLoaderClass.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureClass.h"

#include "Camera.h"

#include <cstring>
class BC_Bullet
{
public:
	BC_Bullet();
	~BC_Bullet();

	//vertex/texture coord/normal containers for the model
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	//interleaved vertex, texture, normal data
	//more efficient than separate VBO's
	std::vector<GLfloat> modelData;

	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	//GLuint EBO;
	//shader for the model
	Shader vsh, fsh;
	GLuint shaderProgram;

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

	glm::vec3 direction;
	glm::vec3 position;

	glm::mat4 normalMatrix;

	float speed;
	float movAngle;

	int radius = 0.2f;

	bool collided = false;
	bool firstUpdate = false;

	int animationCalls = 0;

	void init(int w, int h, std::string modelPath, std::string texturePath, float offsetX, float offsetY, int lightingModel, float angle);
	void update(GLuint elapsedTime, Camera cam, float bX_r, float bX_l, float bY_t, float bY_b);
	void render();
	void setBuffers();
	void borderCollision(float bX_r, float bX_l, float bY_t, float bY_b);
};

