#include "SimpleMinesweeper.h"


SimpleMinesweeper::SimpleMinesweeper()
{
	sAppName = "Minesweeper";
}


bool SimpleMinesweeper::OnUserCreate()
{
	state = State::MENU;

	numButtons = 3;
	buttonHeight = 90;
	buttonWidth = ScreenWidth() / 1.5;
	yPadding = (ScreenHeight() - numButtons * buttonHeight) / (numButtons + 1);
	xPadding = (ScreenWidth() - buttonWidth) / 2;
	for (int i = 1; i <= numButtons; i++)
		buttons.push_back(i * yPadding + (i - 1) * buttonHeight); //calculates y position of each button

	auto func  = [&]() { state = State::STOPPED; difficulty = Difficulty::EASY;   initTiles(); };
	auto func2 = [&]() { state = State::STOPPED; difficulty = Difficulty::MEDIUM; initTiles(); };
	auto func3 = [&]() { state = State::STOPPED; difficulty = Difficulty::HARD;   initTiles(); };
	button  = new Button(xPadding, buttons[0], buttonWidth, buttonHeight, "Easy",   func );
	button2 = new Button(xPadding, buttons[1], buttonWidth, buttonHeight, "Medium", func2);
	button3 = new Button(xPadding, buttons[2], buttonWidth, buttonHeight, "Hard",   func3);

	//load images
	bombImage = std::make_unique<olc::Sprite>("./resources/bomb.png");
	flagImage = std::make_unique<olc::Sprite>("./resources/flag.png");
	tileImage = std::make_unique<olc::Sprite>("./resources/tile.png");
	return true;
}


bool SimpleMinesweeper::OnUserUpdate(float fElapsedTime)
{
	switch (state)
	{
	case State::MENU:
		drawMenu();
		break;

	case State::RUNNING:
		keyInput();
		drawGrid();
		break;

	case State::STOPPED:
		keyInput();
		drawGrid();
		break;

	case State::WON:
		keyInput();
		drawGrid();
		break;

	case State::LOST:
		keyInput();
		drawGrid();
		break;
	}
	return true;
}


void SimpleMinesweeper::initTiles()
{
	tiles.clear();
	//determine variables based on difficulty
	squareWidth = 32;
	rows = (ScreenHeight() / squareWidth) - 3; columns = (ScreenWidth() / squareWidth) - 2;
	numTiles = rows * columns;
	switch (difficulty)
	{
	case Difficulty::EASY:
		numBombs = rows * columns / 7;
		break;
	case Difficulty::MEDIUM:
		numBombs = rows * columns / 6;
		break;
	case Difficulty::HARD:
		numBombs = rows * columns / 5;
		break;
	}
	//reserve space in lists
	tiles.reserve(columns);
	for (auto& row : tiles)
		row.reserve(rows);

	//place tiles in list
	for (int i = 0, bombsPlaced = 0; i < rows; i++) {
		tiles.emplace_back();
		for (int j = 0; j < columns; j++, bombsPlaced++) {
			if (bombsPlaced < numBombs)
				tiles[i].emplace_back(true);
			else
				tiles[i].emplace_back(false);
		}
	}

	//randomize location of bombs by swapping location of tiles a bunch of times
	srand(time(0));
	for (int i = 0; i < numTiles * 100; i++)
		std::swap(tiles[rand() % rows][rand() % columns], tiles[rand() % rows][rand() % columns]);

	//calculate numNearBombs
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			if (tiles[row][column].isBomb)
				modifySurrounding(row, column, [&](int row, int column) { tiles[row][column].numNearBombs++; });
		}
	}
}


void SimpleMinesweeper::drawMenu()
{
	Clear(olc::VERY_DARK_BLUE);
	button->drawButton(this);
	button2->drawButton(this);
	button3->drawButton(this);

	int32_t mouseX = GetMouseX();
	int32_t mouseY = GetMouseY();
	button ->update(mouseX, mouseY, this);
	button2->update(mouseX, mouseY, this);
	button3->update(mouseX, mouseY, this);
}


void SimpleMinesweeper::modifySurrounding(int row, int column, const std::function<void(int, int)>& func) {
	//left
	if (column - 1 >= 0) {
		func(row, column - 1);
		//top right, bottom right
		if (row - 1 >= 0)   func(row - 1, column - 1);
		if (row + 1 < rows) func(row + 1, column - 1);
	}
	//right
	if (column + 1 < columns) {
		func(row, column + 1);
		//top left, bottom left
		if (row - 1 >= 0)   func(row - 1, column + 1);
		if (row + 1 < rows) func(row + 1, column + 1);
	}
	//top, bottom
	if (row - 1 >= 0)   func(row - 1, column);
	if (row + 1 < rows) func(row + 1, column);
}


void SimpleMinesweeper::clearOpenArea(int row, int column)
{
	auto func = [&](int row, int column) {	//recursively digs all tiles that are 0 and tiles that are next to 0
		if (tiles[row][column].numNearBombs == 0 && !tiles[row][column].isPressed) {
			tiles[row][column].isPressed = true;
			clearOpenArea(row, column);
		}
		tiles[row][column].isPressed = true;
	};
	modifySurrounding(row, column, func);
}


void SimpleMinesweeper::keyInput()
{
	bool leftClick = GetMouse(0).bPressed;
	bool rightClick = GetMouse(1).bPressed;
	if (leftClick || rightClick) {
		if (state == State::STOPPED)
			state = State::RUNNING;
		//check if mouse is within grid
		if (GetMouseY() < ScreenHeight() - squareWidth && GetMouseY() > 2 * squareWidth && GetMouseX() < ScreenWidth() - squareWidth && GetMouseX() > squareWidth) {
			int row = ((GetMouseY() - 2 * squareWidth) / squareWidth);							//calculate which tile has been pressed
			int column = ((GetMouseX() - squareWidth) / squareWidth);

			if (leftClick && !tiles[row][column].isFlagged) {									//dig if mouse 0 click and tile is not flagged
				tiles[row][column].isPressed = true;
				if (tiles[row][column].isBomb)
					state = State::LOST;
				if (tiles[row][column].numNearBombs == 0)
					clearOpenArea(row, column);
			}
			else
				tiles[row][column].isFlagged = (!tiles[row][column].isFlagged) ? true : false;  //toggles flag
		}
	}
	if (IsFocused()) {	//reset
		if (GetKey(olc::Key::ESCAPE).bPressed)
			OnUserCreate();
	}
}


void SimpleMinesweeper::drawGrid()
{
	Clear(olc::WHITE);

	//draw tiles
	int y = 0, x = 0;
	int xBuffer, yBuffer;
	for (const auto& column : tiles) {
		x = 0;
		y++;
		for (const auto& row : column) {
			if (row.isPressed) {
				if (row.isBomb) {
					xBuffer = x * squareWidth + squareWidth;
					yBuffer = y * squareWidth + squareWidth;
					DrawSprite(olc::vi2d(xBuffer, yBuffer), bombImage.get());
				}
				else {
					xBuffer = x * squareWidth + squareWidth + 8;
					yBuffer = y * squareWidth + squareWidth + 8;
					if (row.numNearBombs != 0)
						DrawString(xBuffer, yBuffer, std::to_string(row.numNearBombs), getNumColour(row), 2);
				}
			}
			else if (row.isFlagged) 
				DrawSprite(olc::vi2d(x * squareWidth + squareWidth, y * squareWidth + squareWidth), flagImage.get());
			else 
				DrawSprite(olc::vi2d(x * squareWidth + squareWidth, y * squareWidth + squareWidth), tileImage.get());
			x++;
		}
	}
	
	//draw lines
	for (int i = 1; i < ScreenWidth() / squareWidth; i++)
		DrawLine(i * squareWidth, 2 * squareWidth, i * squareWidth, ScreenHeight() - squareWidth, olc::BLACK);
	//horizontal lines
	for (int i = 2; i < ScreenHeight() / squareWidth; i++)
		DrawLine(squareWidth, i * squareWidth, ScreenWidth() - squareWidth, i * squareWidth, olc::BLACK);

	DrawString(squareWidth, squareWidth, "Bombs:" + std::to_string(numBombs - getTilesFlagged()), olc::RED, 3);
}


olc::Pixel SimpleMinesweeper::getNumColour(Tile tile) {
	switch (tile.numNearBombs)
	{
	case 1:
		return olc::BLUE;
		break;
	case 2:
		return olc::GREEN;
		break;
	case 3:
		return olc::RED;
		break;
	case 4:
		return olc::DARK_MAGENTA;
		break;
	case 5:
		return olc::VERY_DARK_RED;
		break;
	case 6:
		return olc::CYAN;
		break;
	case 7:
		return olc::MAGENTA;
		break;
	default:
		return olc::GREY;
		break;
	}
}


int SimpleMinesweeper::getTilesFlagged()
{
	int total = 0;
	for (const auto& row : tiles) {
		for (const auto& tile : row)
			if (tile.isFlagged)
				total++;
	}
	return total;
}


int main() {
	SimpleMinesweeper app;
	if (app.Construct(960, 800, 1, 1))
		app.Start();
	return 0;
}