#include "Clock.h"

Clock::Clock(int x, int y, olc::Pixel colour, int scale) 
	: sec(0), min(0), hour(0), fAccumulatedTime(0), x(x), y(y), colour(colour), scale(scale) {};

void Clock::updateClock(float fElapsedTime) {
	fAccumulatedTime += fElapsedTime;
	sec = fAccumulatedTime;
	if (sec >= 60) {
		fAccumulatedTime = 0;
		sec = 0;
		min++;
	}
	if (min == 60) {
		min = 0;
		hour++;
	}
}

void Clock::reset() {
	sec = 0;
	min = 0;
	hour = 0;
	fAccumulatedTime = 0;
}

std::string Clock::getTime() {
	std::string sSec, sMin, sHour;
	if (sec < 10)
		sSec = "0" + std::to_string(sec);
	else
		sSec = std::to_string(sec);

	if (min < 10)
		sMin = "0" + std::to_string(min);
	else
		sMin = std::to_string(min);

	if (hour < 10)
		sHour = "0" + std::to_string(hour);
	else
		sHour = std::to_string(hour);

	return sHour + ":" + sMin + ":" + sSec;
}


void Clock::drawClock(olc::PixelGameEngine* screen, float fElapsedTime)
{
	updateClock(fElapsedTime);
	screen->DrawString(x, y, this->getTime(), colour, scale);
}
