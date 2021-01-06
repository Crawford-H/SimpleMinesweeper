#pragma once

struct Tile
{
	int numNearBombs = 0;
	bool isPressed = false;
	bool isFlagged = false;
	bool isBomb;

	Tile(bool isBomb) : isBomb(isBomb) {};
};