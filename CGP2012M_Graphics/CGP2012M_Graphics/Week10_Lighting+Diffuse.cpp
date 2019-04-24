//#include <cstdio>
//#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"
#include "Triangle_T.h"
#include "Circle.h"
#include "CircleTexture.h"
#include "Square.h"
#include "Camera.h"
#include "Cube.h"
#include "Model.h"
#include "ModelLoaderClass.h"
#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H
#ifndef SHADERCLASS_H
#define SHADERCLASS_H

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#ifndef SDL_H
#define SDL_H
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// MY INCLUDES
#include "BC_Square.h"


/* 

TO DO LIST

	3D GEOMETRY:
		- VISIBLE 3D GEOMETRY FOR BUBBLES AND PLAYER !
		- VISIBLE GEOMETRY FOR WORLD BOUNDARIES
		- VISIBLE SKYBOX
		  OBJ MODELS ARE LOADED & TEXTURES ARE LOADED WITHIN MAIN CODE
		- OBJ FILES ARE LOADED WITH TEXTURE AND NORMAL DATA 

	PLAYABLE GAME:
		- CHARACTER CAN MOVE / ROTATE, BUBBLES MOVE !
		- BUBBLES ARE CONSTRAINED WITHIN WORLD BOUNDARIES !
		- BUBBLES ARE DESTROYED WHEN FIRED AT !
		- PLAYER HAS 3 LIVES SHOWN ON A 2D HUD !
		- OTHER GAMEPLAY FEATURE 

	LIGHTING & TEXTURES:
		- ALL GEOMETRY IS TEXTURED 
		  LIGHTING IN SCENE FROM ONE LIGHT SOURCE
		- LIGHTING IN SCENE FROM MULTIPLE SOURCES 
		- DYNAMIC LIGHTING (SPOTLIGHT)
		- DIFFERENT LIGHTING MODELS SELECTED

	SHADERS:
		  SHADERS ARE IN THEIR OWN FILES
		  VARIABLE DATA IS PASSED
		- RANDOM COLORING OF BUBBLES
		  GEOMETRY AND TEXTURES IS MANIPULATED
		- GEOMETRY AND TEXTURES ARE ANIMATED
*/

//***************
//variables
SDL_Event event;
SDL_Window *win;
bool windowOpen = true;
bool isFullScreen = false;
float bubbleSpeed = -0.001f;
float radius;
//screen boundaries for collision tests
float bX_r = 2.0f;
float bX_l = -2.0f;
float bY_t = 1.0f;
float bY_b = -1.0f;
//screen centre
float centreX = 0.0f;
float centreY = 0.0f;
//window aspect/width/height
int w;
int h;
float aspect;
int left;
int newwidth;
int newheight;

//transform matrices
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 normalMatrix;

glm::mat4 translate;
glm::mat4 rotate;
glm::mat4 scale;
////glm::mat4 backgroundTranslate;
////glm::mat4 backgroundScale;
glm::vec3 b_scaleFactor;
glm::mat4 modelRotate;
glm::mat4 modelScale;
glm::mat4 modelTranslate;
float angle = 0;

//create camera
Camera cam;
//loac camera variables
glm::vec3 camPos;
glm::vec3 camTarget;

bool flag = true;

glm::vec3 lightCol;
glm::vec3 lightPosition;
glm::vec3 viewPosition;
float ambientIntensity;

//**************
//function prototypes
void handleInput();

int main(int argc, char *argv[]) {
	//start and initialise SDL
	SDL_Start sdl;
	SDL_GLContext context = sdl.Init();
	win = sdl.win;

	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
	aspect = (float)w / (float)h;

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//*****************************************************
	//OpenGL specific data

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//objects
	//create background square
	BC_Square background;
	background.init(w, h);
	//create model
	//could make this better by specifying the texture in this model header
	Model model;
	//create model loader
	ModelImport modelLoader; 
	modelLoader.LoadOBJ2("..//..//Assets//Models//blenderSphere.obj", model.vertices, model.texCoords, model.normals, model.indices);

	//*********************
	//create texture collection
	//create textures - space for 4, but only using 2
	Texture texArray[4];
	//background texture
	////texArray[0].load("..//..//Assets//Textures//space.png");
	////texArray[0].setBuffers();
	texArray[1].load("..//..//Assets//Textures//deathstar.png");
	texArray[1].setBuffers();

	//OpenGL buffers
	///*background.setBuffers();*/
	model.setBuffers();

	//*****************************************
	//set uniform variables
	int transformLocation;
	////int modelLocation;
	////int viewLocation;
	////int projectionLocation;
	int importModelLocation;
	int importViewLocation;
	int importProjectionLocation;
	///*int backgroundColourLocation;*/
	///*int ambientIntensityLocation;*/
	int modelColourLocation;
	int modelAmbientLocation;
	int lightColLocation;
	int normalMatrixLocation;
	int lightPositionLocation;
	int viewPositionLocation;

	GLuint currentTime = 0;
	GLuint lastTime = 0;
	GLuint elapsedTime = 0;

	//lighting for the model
	//Light position setting
	lightPosition = glm::vec3(1.0f, 0.0f, 0.5f);
	//light colour setting
	// Candle:  r:1.0 g:0.57 b:0.16
	// 100W bulb: r:1.0 g:0.84 b:0.66
	// direct sunlight: r:1.0 g:1.0 b:0.98
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 0.98f);

	//light for the background
	//light distance setting
	ambientIntensity = 1.0f;
	lightCol = glm::vec3(1.0f, 1.0f, 0.98f);

	//initialise transform matrices 
	//perspective (3D) projection
	projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
	//initialise view matrix to '1'
	viewMatrix = glm::mat4(1.0f);

	////backgroundScale = glm::mat4(1.0f);
	////backgroundTranslate = glm::mat4(1.0f);
	modelScale = glm::mat4(1.0f);
	modelRotate = glm::mat4(1.0f);
	modelTranslate = glm::mat4(1.0f);

	//once only scale to background, and translate to centre
	///*b_scaleFactor = { 60.0f, 50.0f, 1.0f };*/
	////backgroundScale = glm::scale(backgroundScale, glm::vec3(b_scaleFactor));
	////backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(0.0f, 0.0f, -2.0f));

	//once only scale and translate to model
	modelScale = glm::scale(modelScale, glm::vec3(0.7f, 0.7f, 0.7f));
	modelTranslate = glm::translate(modelTranslate, glm::vec3(0.0f, 0.0f, -1.0f));

	//*****************************
	//'game' loop
	while (windowOpen)
	{
		//*************************
		//****************************
		// OpenGL calls.
		

		glClearColor(1.0f, 1.0f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//process inputs
		handleInput();
		cam.updateCamera();

		//time
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		//update camera matrix
		//camera only moves side to side, formards and backwards (no rotation)
		// set position, target, up direction
		viewMatrix = glm::lookAt(glm::vec3(cam.camXPos, cam.camYPos, cam.camZPos), cam.cameraTarget, cam.cameraUp);

		//////background
		////glUseProgram(background.shaderProgram);
		//////set background lighting
		////backgroundColourLocation = glGetUniformLocation(background.shaderProgram, "uLightColour");
		////glProgramUniform3fv(background.shaderProgram, backgroundColourLocation, 1, glm::value_ptr(lightCol));
		//////light distance
		////ambientIntensityLocation = glGetUniformLocation(background.shaderProgram, "uAmbientIntensity");
		////glProgramUniform1f(background.shaderProgram, ambientIntensityLocation, ambientIntensity);

		//////set background image
		////modelLocation = glGetUniformLocation(background.shaderProgram, "uModel");
		////glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(backgroundTranslate*backgroundScale));
		////viewLocation = glGetUniformLocation(background.shaderProgram, "uView");
		////glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		////projectionLocation = glGetUniformLocation(background.shaderProgram, "uProjection");
		////glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		////glBindTexture(GL_TEXTURE_2D, texArray[0].texture);
		background.update(cam);
		background.render();

		////set cube
		//glUseProgram(cube.shaderProgram);
		////lighting uniforms
		////get and set light colour and position uniform
		//lightColLocation = glGetUniformLocation(cube.shaderProgram, "lightCol");
		//glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour));
		//lightPositionLocation = glGetUniformLocation(cube.shaderProgram, "lightPosition");
		//glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
		////rotation
		//cubeRotate = glm::rotate(cubeRotate, (float)elapsedTime / 1000, glm::vec3(1.0f, 1.0f, 0.0f));
		//cubeModelLocation = glGetUniformLocation(cube.shaderProgram, "uModel");
		//glUniformMatrix4fv(cubeModelLocation, 1, GL_FALSE, glm::value_ptr(cubeTranslate*cubeRotate*cubeScale));
		//cubeViewLocation = glGetUniformLocation(cube.shaderProgram, "uView");
		//glUniformMatrix4fv(cubeViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		//cubeProjectionLocation = glGetUniformLocation(cube.shaderProgram, "uProjection");
		//glUniformMatrix4fv(cubeProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		////set the normal matrix to send to the vertex shader
		//normalMatrix = (glm::mat3)glm::transpose(glm::inverse(cubeTranslate*cubeRotate*cubeScale));
		////set the normalMatrix in the shaders
		//normalMatrixLocation = glGetUniformLocation(cube.shaderProgram, "uNormalMatrix");
		//glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		//cube.render();

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
		glBindTexture(GL_TEXTURE_2D, texArray[1].texture);
		model.render();

		//set to wireframe so we can see the circles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		SDL_GL_SwapWindow(sdl.win);

	}//end loop

	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(context);

	SDL_Quit();
	return 0;
}

void handleInput()
{
	//*****************************
		//SDL handled input
		//Any input to the program is done here

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			windowOpen = false;
		}
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				std::cout << "Window resized w:" << w << " h:" << h << std::endl;
				SDL_GetWindowSize(win, &w, &h);
				newwidth = h * aspect;
				left = (w - newwidth) / 2;
				glViewport(left, 0, newwidth, h);
				break;
				

			default:
				break;
			}
		}
		
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				//move camera 'forward' in the -ve z direction
				cam.camZPos -= cam.camSpeed;
				break;
			case SDLK_s:
				//move camera 'backwards' in +ve z direction
				cam.camZPos += cam.camSpeed;
				break;
			case SDLK_a:
				//move camera left
				//move camera target with position
				cam.camXPos -= cam.camSpeed;
				cam.camXTarget -= cam.camSpeed;
				break;
			case SDLK_d:
				//move camera right
				//move camera target with position
				cam.camXPos += cam.camSpeed;
				cam.camXTarget += cam.camSpeed;
				break;

			case SDLK_q:
				//move camera up
				cam.camYPos += cam.camSpeed;
				cam.camYTarget += cam.camSpeed;
				break;
			case SDLK_e:
				//move camera down
				cam.camYPos -= cam.camSpeed;
				cam.camYTarget -= cam.camSpeed;
				break;
			case SDLK_v:
				lightPosition.x -= 0.1f;
				break;
			case SDLK_b:
				lightPosition.x += 0.1f;
				break;
			
			}
		}
	}
}
#endif
#endif
#endif
#endif