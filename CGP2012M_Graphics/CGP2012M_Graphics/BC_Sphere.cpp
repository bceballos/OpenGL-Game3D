#include "BC_Sphere.h"



BC_Sphere::BC_Sphere()
{
}


BC_Sphere::~BC_Sphere()
{
}

void BC_Sphere::init(int w, int h,std::string modelPath, std::string texturePath)
{
	//shaders
	//vSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.vert");
	//fSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.frag");
	vSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.vert");
	fSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.frag");

	vSh.getShader(1);
	fSh.getShader(2);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vSh.shaderID);
	glAttachShader(shaderProgram, fSh.shaderID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vSh.shaderID);
	glDeleteShader(fSh.shaderID);

	modelLoader.LoadOBJ2(modelPath.c_str(), vertices, texCoords, normals, indices);
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

void BC_Sphere::update(GLuint elapsedTime, Camera cam)
{
	viewMatrix = glm::lookAt(glm::vec3(cam.camXPos, cam.camYPos, cam.camZPos), cam.cameraTarget, cam.cameraUp);

	////set .obj model
	glUseProgram(model.shaderProgram);
	//lighting uniforms
	//get and set light colour and position uniform
	lightColLocation = glGetUniformLocation(shaderProgram, "lightCol");
	glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour));
	lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
	//rotation
	modelRotate = glm::rotate(modelRotate, (float)elapsedTime / 2000, glm::vec3(0.0f, 1.0f, 0.0f));
	importModelLocation = glGetUniformLocation(shaderProgram, "uModel");
	glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(modelTranslate*modelRotate*modelScale));
	importViewLocation = glGetUniformLocation(shaderProgram, "uView");
	glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	importProjectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	//set the normal matrix to send to the vertex shader
	//Light calculations take place in model-view space
	//So we calculate the normal matrix in that space
	normalMatrix = glm::transpose(glm::inverse(modelTranslate*modelRotate*modelScale * viewMatrix));
	//set the normalMatrix in the shaders
	glUseProgram(model.shaderProgram);
	normalMatrixLocation = glGetUniformLocation(shaderProgram, "uNormalMatrix");
	glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glBindTexture(GL_TEXTURE_2D, texture.texture);
}

void BC_Sphere::render()
{
	model.render();
	//draw the square 
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vertices));
	glBindVertexArray(0);
}

void BC_Sphere::setBuffers()
{
	std::cout << sizeof(modelData[0]) << std::endl;
	std::cout << modelData.size() << std::endl;
	//
	//OpenGL buffers
	glGenBuffers(1, &VBO);
	// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
	glGenVertexArrays(1, &VAO);
	//initialise the index buffer
	//glGenBuffers(1, &this->EBO);

	// Bind Vertex Array Object
	glBindVertexArray(VAO);
	// Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//buffer size from vector: vector size (number of elements) * size of one element
	glBufferData(GL_ARRAY_BUFFER, modelData.size() * sizeof(modelData[0]), &modelData[0], GL_STATIC_DRAW);
	// Then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//set the texture attribute pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//set normal attrib pointer
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//set up the EBO

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);

	//Unbind the VAO
	glBindVertexArray(0);
}
