#include "SimpleMinesweeper.h"


SimpleMinesweeper::SimpleMinesweeper()
{
	sAppName = "Minesweeper";
}


bool SimpleMinesweeper::OnUserCreate()
{
	state = State::MENU;
	return true;
}


bool SimpleMinesweeper::OnUserUpdate(float fElapsedTime)
{
	switch (state)
	{
	case MENU:
		drawMenu();
		break;

	case RUNNING:
		break;

	case STOPPED:
		break;

	case WON:
		break;

	case LOST:
		break;
	}
	return true;
}


void SimpleMinesweeper::initTiles()
{

}


void SimpleMinesweeper::drawMenu()
{
	Clear(olc::GREY);

}


int main() {
	SimpleMinesweeper app;
	if (app.Construct(400, 400, 1, 1))
		app.Start();
	return 0;
}