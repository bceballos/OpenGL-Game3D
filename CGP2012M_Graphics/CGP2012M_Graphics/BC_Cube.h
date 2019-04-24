#pragma once
#include <GL/glew.h>
#include "ShaderClass.h"
#include "TextureClass.h"
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

class BC_Cube
{
public:
	BC_Cube();
	~BC_Cube();

	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	//GLuint EBO;
	//shader for the pyramid
	Shader vsh, fsh;
	GLuint shaderProgram;
	//single texture 
	Texture texture;

	int lightColLocation;
	int lightPositionLocation;

	int cubeModelLocation;
	int cubeViewLocation;
	int cubeProjectionLocation;

	int normalMatrixLocation;

	glm::vec3 lightColour;
	glm::vec3 lightPosition;

	glm::mat4 cubeRotate;
	glm::mat4 cubeScale;
	glm::mat4 cubeTranslate;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::mat4 normalMatrix;

	//define vertices for the Cube
	GLfloat vertices[396] = {
		//vertices			 colour values    texture coords      normals
	 -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,	0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  0.0f,	1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  0.0f,	1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,  0.0f,	1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,   1.0f,0.0f,  0.0f,	0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,   1.0f, 0.0f,  0.0f,	0.0f, 0.0f,    0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,		0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f,0.0f, 0.0f,		1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,		1.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,		1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,		0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,		1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,		1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f,0.0f, 0.0f,		0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,		0.0f, 0.0f,     1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,		1.0f, 1.0f,     0.0f, -1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,		1.0f, 1.0f,     0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 1.0f,     0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,     0.0f, -1.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,		0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,		0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,		0.0f, 0.0f,     0.0f, 1.0f, 0.0f
	};

	void init(int w, int h, std::string texturePath);
	void update(GLuint elapsedTime, Camera cam);
	void render();
	void setBuffers();
};

