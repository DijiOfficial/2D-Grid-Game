#include "pch.h"
#include "Game.h"
#include <iostream>
#include <chrono>

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	InitializeMaze();
	g_TimeStart = std::chrono::system_clock::now();

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
	//UpdateEnemyPos(5);
	UpdateTime();
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
void UpdateTime()
{
	++g_Nrframes;
	if (g_Nrframes % g_RefreshFrame == 0)
	{
		g_TimeEnd = std::chrono::system_clock::now();
		g_DeltaTime = g_TimeEnd - g_TimeStart;
		g_TotalTimePassed += g_DeltaTime.count();
		g_TimeStart = std::chrono::system_clock::now();
	}

	if (g_TotalTimePassed > 0 and g_TotalTimePassed - g_EnnemySpeed >= 0)
	{
		UpdateEnemyPos(5, g_Enemy1);
		g_TotalTimePassed = 0;
	}
}

void InitializeMaze()
{
	for (int i = 0; i < g_NrOfCols; i++)
	{
		for (int j = 0; j < g_NrOfRows; j++)
		{
			g_MazeArray[j][i] = int(MazeEntity::path);
		}
	}
	for (size_t j = 0; j < 5; j++)
	{
		g_MazeArray[j+10][10] = int(MazeEntity::wall);
		g_MazeArray[10][j+10] = int(MazeEntity::wall);
		g_MazeArray[15][j+10] = int(MazeEntity::wall);
		g_MazeArray[j+10][15] = int(MazeEntity::wall);
	}

	g_MazeArray[25][25] = int(MazeEntity::wall);
	g_MazeArray[20][25] = int(MazeEntity::wall);
	g_MazeArray[25][20] = int(MazeEntity::wall);
	g_MazeArray[20][20] = int(MazeEntity::wall);

	//for (size_t i = 1; i < 10; i++)
	//{
	//	g_MazeArray[i][2] = int(MazeEntity::path);
	//}
	g_MazeArray[9][1] = int(MazeEntity::endPoint);
	g_Enemy1.x = 18;
	g_Enemy1.y = 18;
	g_Enemy1.currDir = Direction::right;
	g_MazeArray[g_Enemy1.x][g_Enemy1.y] = int(MazeEntity::enemy);
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
			FillRect(g_BlockSizeX * i, g_BlockSizeY * j, g_BlockSizeX, g_BlockSizeY);
			SetColor(1.f, 1.f, 1.f);
			DrawRect(g_BlockSizeX * i, g_BlockSizeY * j, g_BlockSizeX, g_BlockSizeY);
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

void MoveEntity(const Direction& dir, Player& entity)
{
	entity.currDir = dir;
	if (dir == Direction::left || dir == Direction::right)
	{
		if (g_MazeArray[entity.x + int(dir)][entity.y] == int(MazeEntity::path))
		{
			g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
			entity.x += int(dir);
			entity.totalMovement += int(dir);
		}
		else if (g_MazeArray[entity.x + int(dir)][entity.y] == int(MazeEntity::endPoint))
			std::cout << "You won!";

	}
	else if (dir == Direction::up || dir == Direction::down)
	{
		if (g_MazeArray[entity.x][entity.y + int(dir) / 2] == int(MazeEntity::path))
		{
			g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
			entity.y += int(dir) / 2;
			entity.totalMovement += int(dir) / 2;

		}
		else if (g_MazeArray[entity.x][entity.y + int(dir) / 2] == int(MazeEntity::endPoint))
			std::cout << "You won!";
	}
}
bool IsMazeCellPLayerOrPath(Player& entity)
{
	bool firstCondition{}, secondCondition{};
	switch (entity.currDir)
	{
		case Direction::up:
		case Direction::down:
			firstCondition = g_MazeArray[entity.x ][entity.y + int(entity.currDir) / 2] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::player1);
			break;
		case Direction::left:
		case Direction::right:
			firstCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::player1);
			break;
		default:
			break;
	}
	std::cout << "Entity at pos + 1y: " << g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] << ", first:" << firstCondition << ",second: " << secondCondition << std::endl;
	return firstCondition or secondCondition;
}

bool IsEnemyPassedPLayer(Player& entity)
{
	return g_Player1.x == entity.x or g_Player1.y == entity.y;
}

void SwitchEntityDirection(Player& entity)
{
	if (g_Player1.x > entity.x) entity.currDir = Direction::right;
	else if (g_Player1.x < entity.x) entity.currDir = Direction::left;
	else if (g_Player1.y > entity.y) entity.currDir = Direction::up;
	else if (g_Player1.y < entity.y) entity.currDir = Direction::down;
}

bool IsDirectionCorrect(Player& entity)
{
	if (g_Player1.x > entity.x and entity.currDir == Direction::right) return true;
	else if (g_Player1.x < entity.x and entity.currDir == Direction::left) return true ;
	else if (g_Player1.y > entity.y and entity.currDir == Direction::up) return true ;
	else if (g_Player1.y < entity.y and entity.currDir == Direction::down) return true;
	return false;
}

void UpdateEnemyPos(const int movement, Player& entity)
{
	if (sqrt(pow(g_Player1.x - entity.x, 2) + pow(g_Player1.y - entity.y, 2)) <= 5 and not entity.isFollowing) entity.isFollowing = true, SwitchEntityDirection(entity);
	else if (sqrt(pow(g_Player1.x - entity.x, 2) + pow(g_Player1.y - entity.y, 2)) >= 20) entity.isFollowing = false;
	if (entity.isFollowing)
	{
		if (!IsMazeCellPLayerOrPath(entity) or IsEnemyPassedPLayer(entity)) SwitchEntityDirection(entity);
		else if (!IsMazeCellPLayerOrPath(entity))
		{
			switch (entity.currDir)
				{
					case Direction::up:
					case Direction::down:
						if (IsMazeCellPLayerOrPath(entity)) MoveEntity(Direction::right, entity);
						else MoveEntity(Direction::left, entity);
						break;
					case Direction::left:
					case Direction::right:
						if (IsMazeCellPLayerOrPath(entity)) MoveEntity(Direction::up, entity);
						else MoveEntity(Direction::down, entity);
						break;
					default:
						break;
				}
		}
		if (!IsDirectionCorrect(entity)) SwitchEntityDirection(entity);
		MoveEntity(entity.currDir, entity);
	}
	else
	{
		switch (entity.currDir)
		{
			case Direction::up:
				MoveEntity(Direction::up, entity);
				if (entity.totalMovement == 0 or abs(entity.totalMovement) == movement) entity.currDir = Direction::down;
				break;
			case Direction::down:
				MoveEntity(Direction::down, entity);
				if (entity.totalMovement == 0 or abs(entity.totalMovement) == movement) entity.currDir = Direction::up;
				break;
			case Direction::right:
				MoveEntity(Direction::right, entity);
				if (entity.totalMovement == 0 or abs(entity.totalMovement) == movement) entity.currDir = Direction::left;
				break;
			case Direction::left:
				MoveEntity(Direction::left, entity);
				if (entity.totalMovement == 0 or abs(entity.totalMovement) == movement) entity.currDir = Direction::right;
				break;
			default:
				break;
		}
	}
	g_MazeArray[entity.x][entity.y] = int(MazeEntity::enemy);
}
#pragma endregion ownDefinitions