#include "BC_Cube.h"



BC_Cube::BC_Cube()
{
}


BC_Cube::~BC_Cube()
{
}

void BC_Cube::init(int w, int h, std::string texturePath)
{
	//shaders
	//vSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_A.vert");
	//fSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_A.frag");
	vsh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.vert");
	fsh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.frag");
	//vSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_ADS.vert");
	//fSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_ADS.frag");

	vsh.getShader(1);
	fsh.getShader(2);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vsh.shaderID);
	glAttachShader(shaderProgram, fsh.shaderID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vsh.shaderID);
	glDeleteShader(fsh.shaderID);

	lightPosition = glm::vec3(1.0f, 0.0f, 0.5f);
	lightColour = glm::vec3(1.0f, 1.0f, 0.98f);

	cubeRotate = glm::mat4(1.0f);
	cubeScale = glm::mat4(1.0f);
	cubeTranslate = glm::mat4(1.0f);

	cubeScale = glm::scale(cubeScale, glm::vec3(0.7f, 0.7f, 0.7f));
	cubeTranslate = glm::translate(cubeTranslate, glm::vec3(0.0f, 0.0f, -1.0f));

	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);

	//load the texture file
	texture.load(texturePath.c_str());
}

void BC_Cube::update(GLuint elapsedTime, Camera cam)
{
	viewMatrix = glm::lookAt(glm::vec3(cam.camXPos, cam.camYPos, cam.camZPos), cam.cameraTarget, cam.cameraUp);
	//set cube
	glUseProgram(shaderProgram);
	//lighting uniforms
	//get and set light colour and position uniform
	lightColLocation = glGetUniformLocation(shaderProgram, "lightCol");
	glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour));
	lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPosition");
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
	//rotation
	cubeRotate = glm::rotate(cubeRotate, (float)elapsedTime / 1000, glm::vec3(1.0f, 1.0f, 0.0f));
	cubeModelLocation = glGetUniformLocation(shaderProgram, "uModel");
	glUniformMatrix4fv(cubeModelLocation, 1, GL_FALSE, glm::value_ptr(cubeTranslate*cubeRotate*cubeScale));
	cubeViewLocation = glGetUniformLocation(shaderProgram, "uView");
	glUniformMatrix4fv(cubeViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	cubeProjectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(cubeProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	//set the normal matrix to send to the vertex shader
	normalMatrix = (glm::mat3)glm::transpose(glm::inverse(cubeTranslate*cubeRotate*cubeScale));
	//set the normalMatrix in the shaders
	normalMatrixLocation = glGetUniformLocation(shaderProgram, "uNormalMatrix");
	glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void BC_Cube::render()
{
	//specify the shader program and texture
	glUseProgram(shaderProgram);
	glBindTexture(GL_TEXTURE_2D, texture.texture);
	//draw the cube
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void BC_Cube::setBuffers()
{
	//
		//OpenGL buffers
		//set up 1 for the cube
	glGenBuffers(1, &VBO);
	// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
	glGenVertexArrays(1, &VAO);
	//initialise the index buffer
	//glGenBuffers(1, &this->EBO);
	// Bind Vertex Array Object
	glBindVertexArray(VAO);
	// Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//set the colour attribute pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//set texture attrib pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//set normal attrib pointer
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	//Unbind the VAO
	glBindVertexArray(0);

	//texture buffers
	texture.setBuffers();
}
