#pragma once

#include <iostream>
#include <functional>
#include "../resources/olcPixelGameEngine.h"



class Button
{
public:
	Button();
	Button(int x, int y, int length, int width, std::string label, const std::function<void()>& func);
	void drawButton(olc::PixelGameEngine* screen);
	void update(int32_t mouseX, int32_t mouseY, olc::PixelGameEngine* screen);

private:
	int x, y, width, height;
	std::string label;
	std::function<void()> action;

	int yTextPadding, xTextPadding;
	int textScale = 4;
	olc::Pixel bColour = olc::DARK_BLUE, bHighlighted = olc::YELLOW;
	olc::Pixel tColour = olc::WHITE, tHighlighted = olc::BLACK;
	bool highlighted;
};