//#include <cstdio>
//#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"
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
#include "BC_Cube.h"
#include "BC_Framelock.h"
#include "BC_Player.h"
#include "BC_SphereManager.h"
#include "BC_Skybox.h"
#include "BC_Boundary.h"
#include "BC_Bullet.h"

#include <vector>

/* 

TO DO LIST

	3D GEOMETRY:
		  VISIBLE 3D GEOMETRY FOR BUBBLES AND PLAYER 
		  VISIBLE GEOMETRY FOR WORLD BOUNDARIES
		  VISIBLE SKYBOX https://learnopengl.com/Advanced-OpenGL/Cubemaps
		  OBJ MODELS ARE LOADED & TEXTURES ARE LOADED WITHIN MAIN CODE
		  OBJ FILES ARE LOADED WITH TEXTURE AND NORMAL DATA 

	PLAYABLE GAME:
		  CHARACTER CAN MOVE / ROTATE, BUBBLES MOVE 
		  BUBBLES ARE CONSTRAINED WITHIN WORLD BOUNDARIES 
		  BUBBLES ARE DESTROYED WHEN FIRED AT 
		  PLAYER HAS 3 LIVES SHOWN ON A 2D HUD 
		  OTHER GAMEPLAY FEATURE 

	LIGHTING & TEXTURES:
		  ALL GEOMETRY IS TEXTURED 
		  LIGHTING IN SCENE FROM ONE LIGHT SOURCE
		  LIGHTING IN SCENE FROM MULTIPLE SOURCES 
		  DYNAMIC LIGHTING (SPOTLIGHT)
		  DIFFERENT LIGHTING MODELS SELECTED

	SHADERS:
		  SHADERS ARE IN THEIR OWN FILES
		  VARIABLE DATA IS PASSED
		  RANDOM COLORING OF BUBBLES
		  GEOMETRY AND TEXTURES IS MANIPULATED
*/

//***************
//variables
SDL_Event event;
SDL_Window *win;
bool windowOpen = true;
bool isFullScreen = false;
bool lightModel = false;
float bubbleSpeed = -0.001f;
float radius;
//screen boundaries for collision tests
float bX_r = 6.3f;
float bX_l = -6.3f;
float bY_t = 5.4f;
float bY_b = -5.4f;
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
bool isFullscreen = false;
Uint32 setFullscreen = SDL_WINDOW_FULLSCREEN;

glm::vec3 lightCol;
glm::vec3 viewPosition;
float ambientIntensity;


//**************
//function prototypes
void handleInput();

BC_Player player;
BC_SphereManager bubbles;
BC_Boundary borders;
BC_Square life;
BC_Skybox skybox;
//BC_Bullet bullet;

std::vector<BC_Square> lives;
std::vector<BC_Bullet> bullets;

int lighting;
bool lightingSelect = false;

int initW;
int initH;

int main(int argc, char *argv[]) {
	//start and initialise SDL
	SDL_Start sdl;
	SDL_GLContext context = sdl.Init();
	win = sdl.win;

	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
	aspect = (float)w / (float)h;
	initW = w;
	initH = h;

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//*****************************************************
	//OpenGL specific data

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	do {
		std::cout << "ENTER 1 FOR PHONG LIGHTING OR 2 FOR TOON LIGHTING \n";
		std::cin >> lighting;
		try {
			throw lighting;
		}
		catch (int e) {
			if (typeid(lighting).name() != typeid(int).name()) {
				std::cout << "INVALID INPUT PLEASE TRY AGAIN" << std::endl;
			}
		}
	} while (typeid(lighting).name() != typeid(int).name());

	lighting -= 1;


	skybox.init(w, h);
	bubbles.init(5, w, h, lighting);
	player.init(w, h, "..//..//Assets//Models//blenderCube.obj", "..//..//Assets//Textures//deathstar.png", lighting);
	borders.init(w, h, "..//..//Assets//Models//blenderBoundary.obj", "..//..//Assets//Textures//carbon-fibre-seamless-texture.jpg", 0.0f, 0.0f, lighting);
	//bullet.init(w, h, "..//..//Assets//Models//blenderCube.obj", "..//..//Assets//Textures//deathstar.png", player.position.x, player.position.y, lighting, player.movAngle);

	for (int i = 0; i < player.health; i++) {
		lives.push_back(life);
		lives[i].init(1.0f, 1.0f, 1.0f, w, h, -0.2f + (i * 0.2f), -5.5f, -2.0f, "..//..//Assets//Textures//livesHeart.png");
	}

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
		BC_Fpslock t = BC_Fpslock();
		if (player.health == 0) {
			windowOpen = false;
		}
		//*************************
		//****************************
		// OpenGL calls.
		

		glClearColor(1.0f, 1.0f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//process inputs
		handleInput();

		cam.camXTarget = player.position.x;
		cam.camYTarget = player.position.y;
		cam.camZTarget = player.position.z;
		cam.updateCamera();

		//time
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		//update camera matrix
		//camera only moves side to side, formards and backwards (no rotation)
		// set position, target, up directio

		skybox.update(cam);
		skybox.render();

		borders.update(cam);
		borders.render();

		for (int i = 0; i < player.health; i++) {
			lives[i].update(cam);
			lives[i].render();
		}

		//borders.update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b);
		//borders.render();

		//sphere.update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b);
		//sphere.render();

		bubbles.update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b, player, bullets);
		/*bubbles.render();*/

		for (int i = 0; i < bullets.size(); i++) {
			bullets[i].update(elapsedTime, cam, bX_r, bX_l, bY_t, bY_b);
			if (!bullets[i].collided) {
				bullets[i].render();
			}
			else {
				bullets.erase(bullets.begin() + i);
			}
		}

		player.update(cam);
		player.render();

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
				newheight = w / aspect;
				left = (w - newwidth) / 2;
				if (w < newwidth) {
					SDL_SetWindowSize(win, newwidth, h);
					left = 0;
				}
				glViewport(left, 0, newwidth, h);
				SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				break;
			}
		}

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{

			case SDLK_F10:
				
				isFullscreen = SDL_GetWindowFlags(win) & setFullscreen;
				SDL_SetWindowFullscreen(win, isFullscreen ? 0 : setFullscreen);
				if (!isFullscreen) {
					SDL_SetWindowSize(win, initW, initH);
					glViewport(left, 0, initW, initH);
				}
				SDL_SetWindowPosition(win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

				break;

			case SDLK_q:
				//move camera 'forward' in the -ve z direction
				cam.camZPos -= cam.camSpeed;
				break;
			case SDLK_e:
				//move camera 'backwards' in +ve z direction
				cam.camZPos += cam.camSpeed;
				break;
			case SDLK_l:
				lightModel = !lightModel;
				break;
			case SDLK_SPACE:
				if (bullets.size() == 0) {
					std::cout << "BULLET HAS BEEN ADDED \n";
					BC_Bullet attempt;
					bullets.push_back(attempt);
					bullets.back().init(w, h, "..//..//Assets//Models//blenderSphere.obj", "..//..//Assets//Textures//sun.png", player.position.x, player.position.y, lighting, player.movAngle);
				}
				break;
			}
		}
		
		player.input(event);
	}
}
#endif
#endif
#endif
#endif