#pragma once

//STD includes
#include <iostream>
#include <vector>
#include <random>
#include <functional>

//PixelGameEngine includes
#define OLC_PGE_APPLICATION
#include "../Resources/olcPixelGameEngine.h"

#include "Tile.h"
#include "Button.h"
#include "Clock.h"


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
	//handle key input
	void keyInput();
	//modifies tiles surrounding a tile
	void modifySurrounding(int row, int column, const std::function<void(int, int)>& func);
	//presses all tiles around a tile next to no bombs recursively
	void clearOpenArea(int row, int column);
	//draws game
	void drawGrid();
	//returns colour to draw number based on numNearBombs
	inline olc::Pixel getNumColour(Tile tile);
	//draws endscreen text
	void drawEndScreen(std::string message);
	//returns number of tiles flagged
	int getTilesFlagged();
	//checks if all non bomb tiles have been dug
	bool isWon();

private:
	//game states
	enum class State { MENU, RUNNING, STOPPED, WON, LOST } state;
	enum class Difficulty { EASY, MEDIUM, HARD } difficulty;

	//textures
	std::unique_ptr<olc::Sprite> tileImage;
	std::unique_ptr<olc::Sprite> flagImage;
	std::unique_ptr<olc::Sprite> bombImage;

	//tile data
	std::vector<std::vector<Tile>> tiles;
	int numTiles = 0, numBombs = 0;
	int rows = 0, columns = 0;
	int squareWidth;

	int numButtons, buttonHeight, buttonWidth; 
	int xPadding, yPadding;
	int textScale;
	std::vector<int> buttons;
	Button* button;
	Button* button2;
	Button* button3;

	std::unique_ptr<Clock> clock;
};