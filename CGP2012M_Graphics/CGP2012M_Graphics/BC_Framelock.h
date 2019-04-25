#ifndef BC_FRAMELOCK
#define BC_FRAMELOCK
class BC_Fpslock
{
public:
	BC_Fpslock();
	~BC_Fpslock();

	void resetTicksTimer();
	void gameLoopDelay();

	int getTicks();

	const int fps = 60;

private:
	const float deltaTime = 1000 / fps;

	int startTicks;
};

#endif;