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
//#include "SDL.h"
#include <cstring>

class BC_Player
{
public:
	BC_Player();
	~BC_Player();

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

	int lightColLocationTwo;
	int lightPositionLocationTwo;

	int importModelLocation;
	int importViewLocation;
	int importProjectionLocation;

	int normalMatrixLocation;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 lightColour;
	glm::vec3 lightPosition;

	glm::vec3 lightColourTwo;
	glm::vec3 lightPositionTwo;

	glm::mat4 modelRotate;
	glm::mat4 modelScale;
	glm::mat4 modelTranslate;

	glm::vec3 direction;
	glm::vec3 position;


	glm::mat4 normalMatrix;

	float speed;
	float movAngle = 1.5708f;
	bool immortal = false;
	bool triggered = false;

	int radius = 0.4f;
	int health = 3;

	float bX_r = 6.3f;
	float bX_l = -6.3f;
	float bY_t = 5.4f;
	float bY_b = -5.4f;

	int w, h;

	int startTicks;

	enum class Direction { UP, NONE };
	enum class Rotation { LEFT, RIGHT, NONE };

	Rotation pRot = Rotation::NONE;
	Direction pMov = Direction::NONE;

	bool shot = false;
	bool prevLightModel = false;

	void init(int w, int h, std::string modelPath, std::string texturePath, int lightingModel);
	void update(Camera cam);
	void render();
	void input(SDL_Event e);
	void setBuffers();
};

