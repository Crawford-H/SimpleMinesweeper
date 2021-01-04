#pragma once

struct Tile
{
	int numNearBombs = 0;
	bool isPressed = false;
	bool isBomb;
	bool isFlagged = false;

	Tile(bool isBomb) : isBomb(isBomb) {};
};