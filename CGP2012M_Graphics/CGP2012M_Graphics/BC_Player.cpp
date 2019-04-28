#include "BC_Player.h"



BC_Player::BC_Player()
{
}


BC_Player::~BC_Player()
{
}

void BC_Player::init(int w, int h, std::string modelPath, std::string texturePath, int lightingModel)
{
	vsh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_ADPlayer.vert");

	if (lightingModel == 0) {
		fsh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_ADPlayer.frag");
	}
	else {
		fsh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_ADPlayer_Toon.frag");
	}
	

	vsh.getShader(1);
	fsh.getShader(2);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vsh.shaderID);
	glAttachShader(shaderProgram, fsh.shaderID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vsh.shaderID);
	glDeleteShader(fsh.shaderID);

	modelLoader.LoadOBJ2(modelPath.c_str(), this->vertices, this->texCoords, this->normals, this->indices);
	texture.load(texturePath.c_str());

	lightPosition = glm::vec3(0.0f, 0.0f, 0.5f);
	lightColour = glm::vec3(1.0f, 1.0f, 0.98f);

	lightPositionTwo = glm::vec3(0.0f, 5.4f, -0.5f);
	lightColourTwo = glm::vec3(0.0f, 1.0f, 1.0f);

	modelRotate = glm::mat4(1.0f);
	modelScale = glm::mat4(1.0f);
	modelTranslate = glm::mat4(1.0f);

	modelScale = glm::scale(modelScale, glm::vec3(0.4f, 0.4f, 0.4f));
	modelTranslate = glm::translate(modelTranslate, glm::vec3(0.0f, 0.0f, -1.0f));


	this->w = w;
	this->h = h;

	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.1f, 100.0f);

	texture.setBuffers();
	setBuffers();
}

void BC_Player::update(Camera cam)
{

	if (pMov == Direction::UP) {
		modelTranslate = glm::translate(modelTranslate, glm::vec3((float)cos(movAngle)*0.05f, (float)sin(movAngle)*0.05f, 0.0f));
	}
	if (pRot == Rotation::LEFT) {
		movAngle += glm::radians(2.0f);
		modelRotate = glm::rotate(modelRotate, glm::radians(2.0f), glm::vec3(0, 0, 1));
	}
	else if (pRot == Rotation::RIGHT) {
		movAngle -= glm::radians(2.0f);
		modelRotate = glm::rotate(modelRotate, glm::radians(-2.0f), glm::vec3(0, 0, 1));
	}
	position = glm::vec3(modelTranslate[3][0], modelTranslate[3][1], modelTranslate[3][2]);

	viewMatrix = glm::lookAt(glm::vec3(cam.camXPos, cam.camYPos, cam.camZPos), cam.cameraTarget, cam.cameraUp);

	if (immortal && triggered) {
		triggered = false;
		startTicks = SDL_GetTicks();
		lightColourTwo = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	if ((SDL_GetTicks() - startTicks >= 30000) && immortal) {
		immortal = false;
		lightColourTwo = glm::vec3(1.0f, 1.0f, 0.98f);
	}

	////set .obj model
	glUseProgram(shaderProgram);
	//lighting uniforms
	//get and set light colour and position uniform
	lightColLocation = glGetUniformLocation(shaderProgram, "lightCol");
	glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour));
	lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));

	lightColLocationTwo = glGetUniformLocation(shaderProgram, "sLightCol");
	glUniform3fv(lightColLocationTwo, 1, glm::value_ptr(lightColourTwo));
	lightPositionLocationTwo = glGetUniformLocation(shaderProgram, "sLightPos");
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPositionTwo));

	//rotation
	//modelRotate = 1.0f; //ADD PLAYER ROTATION
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
	glUseProgram(shaderProgram);
	normalMatrixLocation = glGetUniformLocation(shaderProgram, "uNormalMatrix");
	glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glBindTexture(GL_TEXTURE_2D, texture.texture);

}

void BC_Player::render()
{
	//glUseProgram(shaderProgram);
	//draw the square 
	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * sizeof(vertices));
	glBindVertexArray(0);

}

void BC_Player::input(SDL_Event e)
{
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_w:
			pMov = Direction::UP;
			break;
		case SDLK_a:
			pRot = Rotation::LEFT;
			break;
		case SDLK_d:
			pRot = Rotation::RIGHT;
			break;
		case SDLK_SPACE:
			//shoot();
			break;
		}
	}
	else if (e.type == SDL_KEYUP) {
		switch (e.key.keysym.sym) {
		case SDLK_w:
			pMov = Direction::NONE;
			break;
		case SDLK_a:
			pRot = Rotation::NONE;
			break;
		case SDLK_d:
			pRot = Rotation::NONE;
			break;
		}
	}
}

void BC_Player::setBuffers()
{
	////interleave the vertex/texture/normal data
	for (int i = 0; i < indices.size(); i += 3)
	{
		modelData.push_back(vertices[indices[i]].x);
		modelData.push_back(vertices[indices[i]].y);
		modelData.push_back(vertices[indices[i]].z);
		modelData.push_back(texCoords[indices[i + 1]].x);
		modelData.push_back(texCoords[indices[i + 1]].y);
		modelData.push_back(normals[indices[i + 2]].x);
		modelData.push_back(normals[indices[i + 2]].y);
		modelData.push_back(normals[indices[i + 2]].z);

	}
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
