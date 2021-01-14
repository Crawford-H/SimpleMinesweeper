#pragma once

#include <string>

#include "../resources/olcPixelGameEngine.h"

class Clock 
{
public:
	Clock(int, int, olc::Pixel, int);
	void reset();
	void updateClock(float);
	std::string getTime();
	void drawClock(olc::PixelGameEngine*, float=0);

private:
	int sec;
	int min;
	int hour;
	float fAccumulatedTime;
	int x, y;
	int scale;
	olc::Pixel colour;
};