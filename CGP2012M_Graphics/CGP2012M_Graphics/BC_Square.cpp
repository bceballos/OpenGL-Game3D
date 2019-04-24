#include "BC_Square.h"



BC_Square::BC_Square()
{
}


BC_Square::~BC_Square()
{
}

void BC_Square::init(int w, int h)
{
	//shaders
	vsh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.vert");
	fsh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.frag");

	vsh.getShader(1);
	fsh.getShader(2);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vsh.shaderID);
	glAttachShader(shaderProgram, fsh.shaderID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vsh.shaderID);
	glDeleteShader(fsh.shaderID);

	texture.load("..//..//Assets//Textures//space.png");
	
	ambientIntensity = 1.0f;
	lightCol = glm::vec3(1.0f, 1.0f, 0.98f);

	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);

	viewMatrix = glm::mat4(1.0f);

	backgroundScale = glm::mat4(1.0f);
	backgroundTranslate = glm::mat4(1.0f);

	b_scaleFactor = { 60.0f, 50.0f, 1.0f };

	backgroundScale = glm::scale(backgroundScale, glm::vec3(b_scaleFactor));
	backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(0.0f, 0.0f, -2.0f));

	setBuffers();
}

void BC_Square::update(Camera cam)
{
	viewMatrix = glm::lookAt(glm::vec3(cam.camXPos, cam.camYPos, cam.camZPos), cam.cameraTarget, cam.cameraUp);

	//background
	glUseProgram(shaderProgram);
	//set background lighting
	backgroundColourLocation = glGetUniformLocation(shaderProgram, "uLightColour");
	glProgramUniform3fv(shaderProgram, backgroundColourLocation, 1, glm::value_ptr(lightCol));
	//light distance
	ambientIntensityLocation = glGetUniformLocation(shaderProgram, "uAmbientIntensity");
	glProgramUniform1f(shaderProgram, ambientIntensityLocation, ambientIntensity);

	//set background image
	modelLocation = glGetUniformLocation(shaderProgram, "uModel");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(backgroundTranslate*backgroundScale));
	viewLocation = glGetUniformLocation(shaderProgram, "uView");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	projectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glBindTexture(GL_TEXTURE_2D, texture.texture);
}

void BC_Square::render()
{
	//draw the square 
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BC_Square::setBuffers()
{
	//
//OpenGL buffers
//set up 1 for the triangle
	glGenBuffers(1, &VBO);
	// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
	glGenVertexArrays(1, &VAO);
	//initialise the index buffer
	glGenBuffers(1, &EBO);
	// Bind Vertex Array Object
	glBindVertexArray(VAO);
	// Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sq_vertices), sq_vertices, GL_STATIC_DRAW);
	//set up the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Then set our colour data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//set the texture coordinates attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	//Unbind the VAO
	glBindVertexArray(0);

	texture.setBuffers();
}
