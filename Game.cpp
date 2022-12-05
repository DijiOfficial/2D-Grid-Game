#include "pch.h"
#include "Game.h"

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	InitializeMaze();

}

void Draw()
{
	ClearBackground();
	DrawMaze();
	// Put your own draw statements here

}

void Update(float elapsedSec)
{
	UpdatePlayerPos();
}

void End()
{
	// free game resources here
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{

}

void OnKeyUpEvent(SDL_Keycode key)
{
	//switch (key)
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	////std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//switch (e.button)
	//{
	//case SDL_BUTTON_LEFT:
	//{
	//	//std::cout << "Left mouse button released\n";
	//	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
	//	break;
	//}
	//case SDL_BUTTON_RIGHT:
	//	//std::cout << "Right mouse button released\n";
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	//std::cout << "Middle mouse button released\n";
	//	break;
	//}
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here
void InitializeMaze()
{
	for (size_t i = 1; i < 10; i++)
	{
		g_MazeArray[i][1] = 1;
	}
	g_MazeArray[9][1] = 2;

}
void DrawMaze()
{
	
	for (size_t i = 0; i < g_NrOfRows; i++)
	{
		for (size_t j = 0; j < g_NrOfCols; j++)
		{
			if (g_MazeArray[i][j] == 0)
				SetColor(0.5f, 0.5f, 0.5f);
			else if (g_MazeArray[i][j] == 1)
				SetColor(0.5f, 1.f, 0.5f);
			else if (g_MazeArray[i][j] == 2)
				SetColor(1.f, 1.f, 0.f);
			else if (g_MazeArray[i][j] == 3)
				SetColor(1.f, 0.f, 0.f);
			FillRect(g_BlockSize * i, g_BlockSize * j, g_BlockSize, g_BlockSize);
			SetColor(1.f, 1.f, 1.f);
			DrawRect(g_BlockSize * i, g_BlockSize * j, g_BlockSize, g_BlockSize);
		}
	}
}
void UpdatePlayerPos()
{
	g_MazeArray[g_Player1.x][g_Player1.y] = 3;
}
#pragma endregion ownDefinitions