#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <functional>

#define OLC_PGE_APPLICATION
#include "../Resources/olcPixelGameEngine.h"

#include "Tile.h"

class SimpleMinesweeper : public olc::PixelGameEngine 
{
public:
	//constructor
	SimpleMinesweeper();
	//called when app is created
	bool OnUserCreate() override;
	//called repeatedly
	bool OnUserUpdate(float fElapsedTime) override;

private:
	//init vector(Tiles) elements, called when object created or game restarted
	void initTiles();
	//draws menu
	void drawMenu();

private:
	enum State {
		MENU, RUNNING, STOPPED, WON, LOST
	};
	State state;
};