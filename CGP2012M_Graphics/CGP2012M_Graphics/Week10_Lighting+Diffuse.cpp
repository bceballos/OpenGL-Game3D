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
#include "BC_Sphere.h"

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

glm::mat4 translate;
glm::mat4 rotate;
glm::mat4 scale;


float angle = 0;

//create camera
Camera cam;
//loac camera variables
glm::vec3 camPos;
glm::vec3 camTarget;

bool flag = true;

glm::vec3 lightCol;
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
	background.init(60.f, 50.0f, 1.0f, w, h, 0.0f, 0.0f, -2.0f, "..//..//Assets//Textures//space.png");
	//create model
	//could make this better by specifying the texture in this model header

	BC_Sphere sphere;
	sphere.init(w, h,"..//..//Assets//Models//blenderSphere.obj", "..//..//Assets//Textures//deathstar.png");

	//*****************************************
	//set uniform variables
	int transformLocation;
	int importModelLocation;

	int modelColourLocation;
	int modelAmbientLocation;

	int viewPositionLocation;

	GLuint currentTime = 0;
	GLuint lastTime = 0;
	GLuint elapsedTime = 0;

	//lighting for the model
	//Light position setting

	//light colour setting
	// Candle:  r:1.0 g:0.57 b:0.16
	// 100W bulb: r:1.0 g:0.84 b:0.66
	// direct sunlight: r:1.0 g:1.0 b:0.98


	//light for the background
	//light distance setting
	ambientIntensity = 1.0f;
	lightCol = glm::vec3(1.0f, 1.0f, 0.98f);

	//initialise transform matrices 
	//perspective (3D) projection
	
	//initialise view matrix to '1'

	//once only scale and translate to model


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

		background.update(cam);
		background.render();

		sphere.update(elapsedTime, cam);
		sphere.render();

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
				//lightPosition.x -= 0.1f; //FIX LIGHTPOSITION IN INPUT LATER
				break;
			case SDLK_b:
				//lightPosition.x += 0.1f;
				break;
			
			}
		}
	}
}
#endif
#endif
#endif
#endif