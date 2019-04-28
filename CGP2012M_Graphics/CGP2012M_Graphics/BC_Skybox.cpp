#include "BC_Skybox.h"



BC_Skybox::BC_Skybox()
{
}


BC_Skybox::~BC_Skybox()
{
}

void BC_Skybox::init(int w, int h)
{
	cubeFaces = {
		"..//..//Assets//Textures//skybox//5dim_ft.png",
		"..//..//Assets//Textures//skybox//5dim_bk.png",
		"..//..//Assets//Textures//skybox//5dim_up.png",
		"..//..//Assets//Textures//skybox//5dim_dn.png",
		"..//..//Assets//Textures//skybox//5dim_rt.png",
		"..//..//Assets//Textures//skybox//5dim_lf.png"
	};

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (int i = 0; i < cubeFaces.size(); i++) {
		SDL_Surface* data = IMG_Load(cubeFaces[i].c_str());
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data->w, data->h, 0, GL_RGB, GL_UNSIGNED_BYTE, data->pixels);
		SDL_FreeSurface(data);
		data = NULL;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	vsh.shaderFileName("..//..//Assets//Shaders//skyboxShader.vert");
	fsh.shaderFileName("..//..//Assets//Shaders//skyboxShader.frag");

	vsh.getShader(1);
	fsh.getShader(2);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vsh.shaderID);
	glAttachShader(shaderProgram, fsh.shaderID);
	glLinkProgram(shaderProgram);

	glDeleteShader(vsh.shaderID);
	glDeleteShader(fsh.shaderID);

	viewMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);

	setBuffers();
}

void BC_Skybox::update(Camera cam)
{
	viewMatrix = glm::lookAt(glm::vec3(cam.camXPos, cam.camYPos, cam.camZPos), cam.cameraTarget, cam.cameraUp);

	glUseProgram(shaderProgram);

	

	auto importProjectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void BC_Skybox::render()
{
	glDepthMask(GL_FALSE);
	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void BC_Skybox::setBuffers()
{
	//
	//OpenGL buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, modelData.size() * sizeof(modelData[0]), &modelData[0], GL_STATIC_DRAW);
	// Then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
