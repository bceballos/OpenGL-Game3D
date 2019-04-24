#include "BC_Model.h"



BC_Model::BC_Model()
{
}


BC_Model::~BC_Model()
{
}

void BC_Model::init(int w, int h,std::string modelPath, std::string texturePath)
{
	modelLoader.LoadOBJ2(modelPath.c_str(), model.vertices, model.texCoords, model.normals, model.indices);
	texture.load(texturePath.c_str());

	lightPosition = glm::vec3(1.0f, 0.0f, 0.5f);
	lightColour = glm::vec3(1.0f, 1.0f, 0.98f);

	modelRotate = glm::mat4(1.0f);
	modelScale = glm::mat4(1.0f);
	modelTranslate = glm::mat4(1.0f);

	modelScale = glm::scale(modelScale, glm::vec3(0.7f, 0.7f, 0.7f));
	modelTranslate = glm::translate(modelTranslate, glm::vec3(0.0f, 0.0f, -1.0f));

	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);

	texture.setBuffers();
	model.setBuffers();
}

void BC_Model::update(GLuint elapsedTime, Camera cam)
{
	viewMatrix = glm::lookAt(glm::vec3(cam.camXPos, cam.camYPos, cam.camZPos), cam.cameraTarget, cam.cameraUp);

	////set .obj model
	glUseProgram(model.shaderProgram);
	//lighting uniforms
	//get and set light colour and position uniform
	lightColLocation = glGetUniformLocation(model.shaderProgram, "lightCol");
	glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour));
	lightPositionLocation = glGetUniformLocation(model.shaderProgram, "lightPos");
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
	//rotation
	modelRotate = glm::rotate(modelRotate, (float)elapsedTime / 2000, glm::vec3(0.0f, 1.0f, 0.0f));
	importModelLocation = glGetUniformLocation(model.shaderProgram, "uModel");
	glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(modelTranslate*modelRotate*modelScale));
	importViewLocation = glGetUniformLocation(model.shaderProgram, "uView");
	glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	importProjectionLocation = glGetUniformLocation(model.shaderProgram, "uProjection");
	glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	//set the normal matrix to send to the vertex shader
	//Light calculations take place in model-view space
	//So we calculate the normal matrix in that space
	normalMatrix = glm::transpose(glm::inverse(modelTranslate*modelRotate*modelScale * viewMatrix));
	//set the normalMatrix in the shaders
	glUseProgram(model.shaderProgram);
	normalMatrixLocation = glGetUniformLocation(model.shaderProgram, "uNormalMatrix");
	glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glBindTexture(GL_TEXTURE_2D, texture.texture);
}

void BC_Model::render()
{
	model.render();
}
