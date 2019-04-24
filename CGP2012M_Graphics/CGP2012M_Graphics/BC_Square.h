#pragma once

#include <GL/glew.h>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureClass.h"

#include <array>

#include "ShaderClass.h"

#include <cstring>

#include "Camera.h"

class BC_Square
{
public:
	BC_Square();
	~BC_Square();

	GLuint VBO, VAO, EBO;
	GLuint shaderProgram;

	Shader vsh, fsh;

	int backgroundColourLocation;
	int ambientIntensityLocation;
	int modelLocation;
	int viewLocation;
	int projectionLocation;

	glm::vec3 lightCol;

	float ambientIntensity;

	glm::mat4 backgroundTranslate;
	glm::mat4 backgroundScale;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	//define vertices for the square
	GLfloat sq_vertices[32] = {
		//vertices				colour				texture coordinates    
		0.1f, 0.1f, 0.0f,		 0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
		0.1f, -0.1f, 0.0f,		 0.0f, 1.0f, 0.0f,	 1.0f, 0.0f,
		-0.1f, -0.1f, 0.0f,		0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
		-0.1f, 0.1f, 0.0f,		 0.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	//define indices for the square
	GLuint indices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	Texture texture;

	void init(float xScale, float yScale, float zScale, int w, int h, float xPos, float yPos, float zPos, std::string texturePath);
	void update(Camera cam);
	void render();
	void setBuffers();

};

