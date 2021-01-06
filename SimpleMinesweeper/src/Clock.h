#pragma once

#include <string>

class Clock 
{
public:
	Clock();
	void reset();
	void updateClock(float);
	std::string getTime();

private:
	int sec;
	int min;
	int hour;
	float fAccumulatedTime;
};