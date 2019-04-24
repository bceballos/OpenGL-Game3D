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
class BC_Sphere
{
public:
	BC_Sphere();
	~BC_Sphere();

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

	glm::mat4 normalMatrix;

	void init(int w, int h, std::string modelPath, std::string texturePath);
	void update(GLuint elapsedTime, Camera cam);
	void render();
	void setBuffers();
};

