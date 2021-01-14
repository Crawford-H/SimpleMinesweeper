#include "Button.h"

Button::Button() 
{
	x = 0; y = 0; width = 10; height = 10;
	label = ""; xTextPadding = 0; yTextPadding = 0;
	action = NULL;
	highlighted = false;
}


Button::Button(int x, int y, int width, int height, std::string label, const std::function<void()>& action)
	: x(x), y(y), width(width), height(height), label(label), action(action)
{
	yTextPadding = (height - (8 * textScale)) / 2; //text is scaled by 8 pixels
	xTextPadding = (x + width / 2) - (8 * textScale) * (label.length() / 2); //centers text
	highlighted = false;
}


void Button::drawButton(olc::PixelGameEngine* screen)
{
	if (!highlighted) {
		screen->FillRect(x, y, width, height, bColour);
		screen->DrawString(xTextPadding, y + yTextPadding, label, tColour, textScale);
	}
	else {
		screen->FillRect(x, y, width, height, bHighlighted);
		screen->DrawString(xTextPadding, y + yTextPadding, label, tHighlighted, textScale);
	}
}


void Button::update(int32_t mouseX, int32_t mouseY, olc::PixelGameEngine* screen) 
{
	if (mouseX > x && mouseX < x + width) {
		if (mouseY > y && mouseY < y + height) {
			screen->FillRect(x, y, width, height, bHighlighted);
			screen->DrawString(xTextPadding, y + yTextPadding, label, tHighlighted, textScale);
			if (screen->GetMouse(0).bPressed)
				action();
		}
	}
}