#include "BC_Framelock.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "SDL.h"

BC_Fpslock::BC_Fpslock()
{
	resetTicksTimer();
}

BC_Fpslock::~BC_Fpslock()
{
	gameLoopDelay();
}

void BC_Fpslock::resetTicksTimer()
{
	startTicks = SDL_GetTicks();
}

void BC_Fpslock::gameLoopDelay()
{
	int frameLength = getTicks();

	if (frameLength < deltaTime) {
		SDL_Delay(deltaTime - frameLength);
	}

	frameLength = getTicks();
}

int BC_Fpslock::getTicks()
{
	return (SDL_GetTicks() - startTicks);
}
