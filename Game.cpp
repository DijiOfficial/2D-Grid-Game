#include "pch.h"
#include "Game.h"
#include <iostream>

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
	UpdatePlayer1Pos();
	UpdateEnemyPos();
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
	switch (key)
	{
	case SDLK_LEFT:
		
		MovePlayer(Direction::left);
		break;
	case SDLK_RIGHT:
		
		MovePlayer(Direction::right);
		break;
	case SDLK_UP:
		
		MovePlayer(Direction::up);
		break;
	case SDLK_DOWN:
		
		MovePlayer(Direction::down);
		break;
	}
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
		g_MazeArray[i][1] = int(MazeEntity::path);
	}
	for (size_t i = 1; i < 10; i++)
	{
		g_MazeArray[i][2] = int(MazeEntity::path);
	}
	g_MazeArray[9][1] = int(MazeEntity::endPoint);
	g_Enemy1.x = 9;
	g_Enemy1.y = 2;
}
void DrawMaze()
{

	for (size_t i = 0; i < g_NrOfRows; i++)
	{
		for (size_t j = 0; j < g_NrOfCols; j++)
		{
			if (g_MazeArray[i][j] == int(MazeEntity::wall))
				SetColor(0.5f, 0.5f, 0.5f);
			else if (g_MazeArray[i][j] == int(MazeEntity::path))
				SetColor(0.5f, 1.f, 0.5f);
			else if (g_MazeArray[i][j] == int(MazeEntity::endPoint))
				SetColor(1.f, 1.f, 0.f);
			else if (g_MazeArray[i][j] == int(MazeEntity::player1))
				SetColor(1.f, 0.f, 0.f);
			else if (g_MazeArray[i][j] == int(MazeEntity::enemy))
				SetColor(1.f, 0.f, 1.f);
			FillRect(g_BlockSize * i, g_BlockSize * j, g_BlockSize, g_BlockSize);
			SetColor(1.f, 1.f, 1.f);
			DrawRect(g_BlockSize * i, g_BlockSize * j, g_BlockSize, g_BlockSize);
		}
	}
}
void UpdatePlayer1Pos()
{
	g_MazeArray[g_Player1.x][g_Player1.y] = int(MazeEntity::player1);
}
void MovePlayer(const Direction& dir)
{
	if (dir == Direction::left || dir == Direction::right)
	{	
		if (g_MazeArray[g_Player1.x + int(dir)][g_Player1.y] == int(MazeEntity::path))
		{
			g_MazeArray[g_Player1.x][g_Player1.y] = int(MazeEntity::path);
			g_Player1.x += int(dir);
		}
		else if (g_MazeArray[g_Player1.x + int(dir)][g_Player1.y] == int(MazeEntity::endPoint))
			std::cout << "You won!";

	}
	else if (dir == Direction::up || dir == Direction::down)
	{
		if (g_MazeArray[g_Player1.x][g_Player1.y + int(dir)/2] == int(MazeEntity::path))
		{
			g_MazeArray[g_Player1.x][g_Player1.y] = int(MazeEntity::path);
			g_Player1.y += int(dir)/2 ;
		}
		else if (g_MazeArray[g_Player1.x][g_Player1.y + int(dir)/2 ] == int(MazeEntity::endPoint))
			std::cout << "You won!";
	}
}
void UpdateEnemyPos()
{
	g_MazeArray[g_Enemy1.x][g_Enemy1.y] = int(MazeEntity::enemy);
}
#pragma endregion ownDefinitions