#include "pch.h"
#include "Game.h"
#include <iostream>
#include <chrono>
#include <vector>

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	InitializeTextures();

	TextureFromString(std::to_string(g_LevelNr), "resources/goudysto.ttf", 48, Color4f{ 1.f,0.5f,0.f,0.8f }, g_LevelTexture);
	const int playerStartPosX{ 1 }, playerStartPosY{ 1 };
	InitializeMaze();
	InitializeGameResources(playerStartPosX, playerStartPosY);
	//GenerateNewMaze();   //uncomment for randomGen of first level

	const float scale{ 0.5f };
	g_ButtonRect.left = g_WindowWidth / 2 - g_StartButton.width / 2 * scale;
	g_ButtonRect.bottom = g_WindowHeight / 2 - g_StartButton.height / 2 * scale;
	g_ButtonRect.width = g_StartButton.width * scale;
	g_ButtonRect.height = g_StartButton.height * scale;
}

void Draw()
{
	ClearBackground();

	if (g_IsGameStarted)
	{
		if (IsGameLost())
		{
			DrawTexture(g_LostGamePage, Rectf{ 0.f,0.f,g_WindowWidth,g_WindowHeight });
			DrawTexture(g_RetryButton, g_ButtonRect);
		}
		else
		{
			DrawMaze();
			DrawTexture(g_LevelTexture, Point2f{ 20.f, g_WindowHeight - 80.f });
			DrawBeams();
			if (g_IsInMenu)
				DrawTexture(g_InfoPanel, Point2f{ 0.f,0.f });
		}
	}
	else
	{
		DrawTexture(g_StartPage, Rectf{ 0.f,0.f,g_WindowWidth,g_WindowHeight });
		DrawTexture(g_StartButton, g_ButtonRect);

	}
		
}

void Update(float elapsedSec)
{
	if (g_IsGameStarted && !IsGameLost() && !g_IsInMenu)
	{	
		UpdatePlayerPos(g_Player);
		UpdateTime();
		UpdateBeam();
		KillEnemy();
		KillBoss();
		DeleteBeam();
	}
}

void End()
{
	for (int i = 0; i < g_NrOfRows; ++i)
	{
		delete[] g_MazeArray[i];
	}
	delete[] g_MazeArray;

	for (int i = 0; i < g_BeamArray.size(); i++)
	{
		g_BeamArray.erase(g_BeamArray.begin() + i);
	}

	DeleteTextures();
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	if(!IsGameLost() && !g_IsInMenu)
	{
		switch (key)
		{
		case SDLK_LEFT:
			MoveEntity(Direction::left, g_Player);
			break;
		case SDLK_RIGHT:
			MoveEntity(Direction::right, g_Player);
			break;
		case SDLK_UP:
			MoveEntity(Direction::up, g_Player);
			break;
		case SDLK_DOWN:
			MoveEntity(Direction::down, g_Player);
			break;
		}
	}
}

void OnKeyUpEvent(SDL_Keycode key)
{
	if (key == SDLK_SPACE && !g_IsInMenu)
	{
		const float beamPosX{ (g_Player.x ) * g_BlockSizeX }, beamPosY{ g_Player.y * g_BlockSizeY };
		g_BeamArray.emplace_back(beamPosX, beamPosY);
		g_BeamArray[g_BeamArray.size() - 1].currDir = g_Player.currDir;
	}
	if (key == SDLK_i) g_IsInMenu = !g_IsInMenu;
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

	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
	{	Point2f mousePos{ float(e.x), float(g_WindowHeight - e.y) };
	if (!g_IsGameStarted)
	{
		if (IsPointInRect(mousePos, g_ButtonRect))
			g_IsGameStarted = true;
	}
	if (IsGameLost())
	{
		if (IsPointInRect(mousePos, g_ButtonRect))
		{
			g_LevelNr = 1;
			UpdateLevel(g_LevelNr);
			//const int playerStartPosX{ 1 }, playerStartPosY{ 1 };
			//InitializeGameResources(playerStartPosX, playerStartPosY);
			GenerateNewMaze();
		}
	}
	break;
	}
	}
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Maze generation
void GenerateNewMaze()
{
	//Clearing previous level resources
	ClearAllBeams();
	ClearEnemies();
	g_LevelNr++;
	UpdateLevel(g_LevelNr);
	//generating new maze size
	int cols{ (rand() % 6) * 2 + 25 };
	int rows{ cols };
	g_BlockSizeX = { g_WindowWidth / rows };
	g_BlockSizeY = { g_WindowHeight / cols };

	//clear the player before generating
	g_MazeArray[g_Player.x][g_Player.y] = int(MazeEntity::path);

	//create new array
	int** newMazeArray = new int* [rows];
	for (int i = 0; i < rows; ++i)
	{
		newMazeArray[i] = new int[cols];
	}
	//delete old one
	for (int i = 0; i < g_NrOfRows; ++i)
	{
		delete[] g_MazeArray[i];
	}
	delete[] g_MazeArray;
	//reassign new one
	g_MazeArray = newMazeArray;
	g_NrOfRows = rows;
	g_NrOfCols = cols;
	
	if (g_LevelNr == 2)
	{
		//fill 
		for (int i = 0; i < g_NrOfCols; i++)
		{
			for (int j = 0; j < g_NrOfRows; j++)
			{
				g_MazeArray[j][i] = int(MazeEntity::path);
			}
		}
		for (int i = 0; i < g_NrOfCols; i++)
		{
			g_MazeArray[0][i] = int(MazeEntity::wall);
			g_MazeArray[g_NrOfCols - 1][i] = int(MazeEntity::wall);
			g_MazeArray[i][g_NrOfCols - 1] = int(MazeEntity::wall);
			g_MazeArray[i][0] = int(MazeEntity::wall);
		}
		g_MazeArray[3][3] = int(MazeEntity::wall);
		g_MazeArray[4][3] = int(MazeEntity::wall);
		g_MazeArray[3][4] = int(MazeEntity::wall);
		g_MazeArray[3][g_NrOfCols - 4] = int(MazeEntity::wall);
		g_MazeArray[4][g_NrOfCols - 4] = int(MazeEntity::wall);
		g_MazeArray[3][g_NrOfCols - 5] = int(MazeEntity::wall);

		g_MazeArray[g_NrOfCols - 4][3] = int(MazeEntity::wall);
		g_MazeArray[g_NrOfCols - 5][3] = int(MazeEntity::wall);
		g_MazeArray[g_NrOfCols - 4][4] = int(MazeEntity::wall);
		g_MazeArray[g_NrOfCols - 4][g_NrOfCols - 4] = int(MazeEntity::wall);
		g_MazeArray[g_NrOfCols - 5][g_NrOfCols - 4] = int(MazeEntity::wall);
		g_MazeArray[g_NrOfCols - 4][g_NrOfCols - 5] = int(MazeEntity::wall);

		//entities
		g_Player.x = 1;
		g_Player.y = 1;
		g_Boss.x = g_NrOfCols / 2;
		g_Boss.y = g_NrOfCols / 2;
		g_Boss.isBoss = true;
		g_Boss.isAlive = true;
		g_Boss.health = 4;	
	}
	else
	{
		//fill it
		for (int i = 0; i < g_NrOfCols; i++)
		{
			for (int j = 0; j < g_NrOfRows; j++)
			{
				g_MazeArray[j][i] = int(MazeEntity::wall);
			}
		}
		if (g_LevelNr == 1)
		{
			g_Player.x = 1;
			g_Player.y = 1;
		}
		DepthFirstSearch(g_Player.x, g_Player.y);
		g_MazeArray[g_Player.x][g_Player.y] = int(MazeEntity::player1);

		//generating the endPoint
		int endPointY{}, endPointX{};
		int enemyX{}, enemyY{};
		while (true)
		{
			if (g_Player.x >= g_NrOfRows / 2) endPointX = { rand() % (g_NrOfRows / 2) + 1 };
			else endPointX = { g_NrOfRows / 2 + rand() % (g_NrOfRows / 2 - 1) + 1 };

			if (g_Player.y >= g_NrOfCols / 2) endPointY = { rand() % (g_NrOfCols / 2) + 1 };
			else endPointY = { g_NrOfCols / 2 + rand() % (g_NrOfCols / 2 - 1) + 1 };

			if (GetDistance(float(g_Player.x), float(g_Player.y), float(endPointY), float(endPointX)) > 5)
			{
				if (endPointY % 2 == 1 and endPointX % 2 == 1) break;
			}

		}

		//generate the enemy
		while (true)
		{
			g_Enemy1.x = { rand() % (g_NrOfRows - 2) + 1 };
			g_Enemy1.y = { rand() % (g_NrOfCols - 2) + 1 };

			if (GetDistance(float(g_Player.x), float(g_Player.y), float(g_Enemy1.x), float(g_Enemy1.y)) > 8)
			{
				if (endPointX != g_Enemy1.x or g_Enemy1.y != enemyY) break;
			}

		}

		g_MazeArray[endPointX][endPointY] = int(MazeEntity::endPoint);
		g_Enemy1.isAlive = true;

		//Generate random Paths openings
		for (int i = 0; i < int(4 * (g_NrOfCols + g_NrOfRows) / 5); i++)
		{
			while (true)
			{
				const int randX{ rand() % (g_NrOfRows - 2) + 1 };
				const int randY{ rand() % (g_NrOfCols - 2) + 1 };
				if (g_MazeArray[randX][randY] == int(MazeEntity::wall))
				{
					g_MazeArray[randX][randY] = int(MazeEntity::path);
					break;
				}
			}
		}
	}
}

std::vector<Point2i> getAdjacentArray(int x, int y)
{
	std::vector<Point2i> validPos{};

	if (isValidPos(x - 2, y, g_NrOfRows, g_NrOfCols)) validPos.push_back(Point2i{ x - 2, y });
	if (isValidPos(x, y - 2, g_NrOfRows, g_NrOfCols)) validPos.push_back(Point2i{ x    , y - 2 });
	if (isValidPos(x, y + 2, g_NrOfRows, g_NrOfCols)) validPos.push_back(Point2i{ x    , y + 2 });
	if (isValidPos(x + 2, y, g_NrOfRows, g_NrOfCols)) validPos.push_back(Point2i{ x + 2, y });

	return validPos;
}

bool isValidPos(int x, int y, int rows, int cols)
{
	if (x < 0 || y < 0 || x > rows - 1 || y > cols - 1) return false;
	return true;
}

void DepthFirstSearch(int x, int y)
{
	g_MazeArray[x][y] = int(MazeEntity::path);
	std::vector<Point2i> listOfAdjacent = getAdjacentArray(x, y);
	//intVector.size()
	//array[(i + start) % arraysize]
	int start{ rand() % int(listOfAdjacent.size()) };
	for (int i = 0; i < listOfAdjacent.size(); i++)
	{
		Point2i adjacentCell{ listOfAdjacent[(i + start) % listOfAdjacent.size()].x, listOfAdjacent[(i + start) % listOfAdjacent.size()].y };
		if (g_MazeArray[adjacentCell.x][adjacentCell.y] == int(MazeEntity::wall))
		{
			g_MazeArray[adjacentCell.x][adjacentCell.y] = 1;
			Point2i deltaAdjacentOriginal{ adjacentCell.x - x, adjacentCell.y - y };
			//g_MazeArray[listOfAdjacent[(i + start) % listOfAdjacent.size()].x - 1][listOfAdjacent[(i + start) % listOfAdjacent.size()].y - 1] = 1;//this is likely the problem need to remove the wall between start position and current position
			g_MazeArray[x + deltaAdjacentOriginal.x / 2][y + deltaAdjacentOriginal.y / 2] = 1;//this is likely the problem need to remove the wall between start position and current position

			DepthFirstSearch(listOfAdjacent[(i + start) % listOfAdjacent.size()].x, listOfAdjacent[(i + start) % listOfAdjacent.size()].y);
		}
	}
}

void Display2DArray()
{
	std::cout << "\n Two Dimensional Array is : \n";
	for (int i = 0; i < g_NrOfRows; i++)
	{
		for (int j = 0; j < g_NrOfCols; j++)
		{
			std::cout << " " << g_MazeArray[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void ClearEnemies()
{
	g_Enemy1.isAlive = false;
	g_MazeArray[g_Enemy1.x][g_Enemy1.y] = int(MazeEntity::path);
}

// Update functions
void UpdateTime()
{
	const int refreshFrame{ 100 };
	const float enemySpeed{ 0.5f };
	++g_Nrframes;

	if (g_Nrframes % refreshFrame == 0)
	{
		g_TimeEnd = std::chrono::system_clock::now();
		g_DeltaTime = g_TimeEnd - g_TimeStart;
		g_TotalTimePassed += g_DeltaTime.count();
		g_TimeStart = std::chrono::system_clock::now();
		g_Nrframes = 0;
	}

	//need a function to load and unload them or something and reassign them when a new level is made. maybe inside a dynamic array?
	if (g_Enemy1.isAlive)
	{
		if (g_TotalTimePassed > 0 and g_TotalTimePassed - enemySpeed >= 0)
		{
			UpdateEnemyPos(5, g_Enemy1);
			g_TotalTimePassed = 0;
		}
	}
	if (g_Boss.isAlive)
	{
		if (g_TotalTimePassed > 0 and g_TotalTimePassed - enemySpeed >= 0)
		{
			UpdateEnemyPos(5, g_Boss);
			g_TotalTimePassed = 0;
		}
	}
}

void UpdatePlayerPos(const Entity& player)
{
	g_MazeArray[g_Player.x][g_Player.y] = int(MazeEntity::player1);
}

void UpdateLevel(int level)
{
	TextureFromString(std::to_string(level), "resources/goudysto.ttf", 48, Color4f{ 1.f,0.5f,0.f,0.8f }, g_LevelTexture);
}

void UpdateEnemyPos(const int movement, Entity& entity)
{
	bool hasMovedSideways{ false };
	if (entity.isBoss) entity.isFollowing = true;
	else
	{
		float distBetPlAndEn = GetDistance(float(g_Player.x), float(g_Player.y), float(g_Enemy1.x), float(g_Enemy1.y));
		if (distBetPlAndEn <= 5 and not entity.isFollowing) entity.isFollowing = true, SwitchEntityDirection(entity);
		else if (distBetPlAndEn >= 10) entity.isFollowing = false;
	}
	if (entity.isFollowing)
	{
		if (IsEnemyPassedPLayer(entity)) SwitchEntityDirection(entity);

		if (!IsMazeCellPLayerOrPath(entity))
		{
			hasMovedSideways = true;
			Direction originalDir{ entity.currDir };
			switch (entity.currDir)
			{
				case Direction::down:
				case Direction::up:
					entity.currDir = Direction::left;
					if (IsMazeCellPLayerOrPath(entity)) MoveEntity(Direction::left, entity);
					else MoveEntity(Direction::right, entity);
					entity.currDir = originalDir;
					break;
				case Direction::left:
				case Direction::right:
					entity.currDir = Direction::down;
					if (IsMazeCellPLayerOrPath(entity)) MoveEntity(Direction::down, entity);
					else MoveEntity(Direction::up, entity);
					entity.currDir = originalDir;
					break;
				default:
					break;
			}
		}
		else hasMovedSideways = false;

		if (!IsDirectionCorrect(entity)) SwitchEntityDirection(entity);
		if (!hasMovedSideways) MoveEntity(entity.currDir, entity);
	}
	else
	{
		if (abs(entity.totalMovement) > movement) entity.totalMovement = 0;
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
	if (!entity.isBoss) g_MazeArray[entity.x][entity.y] = int(MazeEntity::enemy);
	else
	{
		g_MazeArray[entity.x][entity.y] = int(MazeEntity::boss);
		g_MazeArray[entity.x + 1][entity.y] = int(MazeEntity::boss);
		g_MazeArray[entity.x][entity.y + 1] = int(MazeEntity::boss);
		g_MazeArray[entity.x + 1][entity.y + 1] = int(MazeEntity::boss);
	}
}

void UpdateBeam()
{
	const float beamSpeed{ 0.2f };
	for (auto& beam : g_BeamArray)
	{
		if (beam.currDir == Direction::left || beam.currDir == Direction::right)
			beam.x += beamSpeed * int(beam.currDir);
		else
			beam.y += beamSpeed * int(beam.currDir) / 2;
	}
}

// Initialize functions
void InitializeMaze()
{

	for (int i = 0; i < g_NrOfRows; ++i)
	{
		g_MazeArray[i] = new int[g_NrOfCols];
	}
	for (int i = 0; i < g_NrOfCols; i++)
	{
		for (int j = 0; j < g_NrOfRows; j++)
		{
			g_MazeArray[j][i] = int(MazeEntity::path);
		}
	}
	for (size_t j = 0; j < 5; j++)							//also dont need this if 1st level randGenerated
	{														//
		g_MazeArray[j + 10][10] = int(MazeEntity::wall);	//
		g_MazeArray[10][j + 10] = int(MazeEntity::wall);	//
		g_MazeArray[15][j + 10] = int(MazeEntity::wall);	//
		g_MazeArray[j + 10][15] = int(MazeEntity::wall);	//
	}														//
															//
	g_MazeArray[9][1] = int(MazeEntity::endPoint);			//

}

void InitializeGameResources(int playerStartPosX, int playerStartPosY)
{
	g_Player.x = playerStartPosX;
	g_Player.y = playerStartPosY;

	g_Enemy1.x = 20; // don t need these if the first level is randomly generated
	g_Enemy1.y = 20; //
	g_Enemy1.currDir = Direction::right; //
	g_MazeArray[g_Enemy1.x][g_Enemy1.y] = int(MazeEntity::enemy); //

	g_TimeStart = std::chrono::system_clock::now();
	g_Player.isPlayableCharacter = true;
	g_Enemy1.isPlayableCharacter = false;
	g_Boss.isAlive = false;
}

void InitializeTextures()
{
	TextureFromFile("resources/start-game-background.jpg", g_StartPage);
	TextureFromFile("resources/game-lost-background.jpg", g_LostGamePage);
	TextureFromFile("resources/start-game-button.png", g_StartButton);
	TextureFromFile("resources/retry-game-button.png", g_RetryButton);

	TextureFromFile("resources/path.jpg", g_PathTexture);
	TextureFromFile("resources/end-point.jpg", g_EndPointTexture);
	TextureFromFile("resources/wall.jpg", g_WallTexture);
	TextureFromFile("resources/enemy-texture.jpg", g_Enemy1.texture);
	TextureFromFile("resources/player-texture.jpg", g_Player.texture);
	TextureFromFile("resources/Boss.png", g_Boss.texture);

	TextureFromFile("resources/beam-texture1.png", g_BeamTexture[0]);
	TextureFromFile("resources/beam-texture2.png", g_BeamTexture[1]);
	TextureFromFile("resources/info-panel.png", g_InfoPanel);
}

void DeleteTextures()
{
	DeleteTexture(g_StartButton);
	DeleteTexture(g_StartPage);
	DeleteTexture(g_RetryButton);
	DeleteTexture(g_WallTexture);
	DeleteTexture(g_PathTexture);
	DeleteTexture(g_EndPointTexture);
	DeleteTexture(g_Player.texture);
	DeleteTexture(g_Enemy1.texture);
	DeleteTexture(g_LostGamePage);
	DeleteTexture(g_LevelTexture);
	DeleteTexture(g_BeamTexture[0]);
	DeleteTexture(g_BeamTexture[1]);
	DeleteTexture(g_InfoPanel);
}

// Draw functions
void DrawMaze()
{

	for (size_t i = 0; i < g_NrOfRows; i++)
	{
		for (size_t j = 0; j < g_NrOfCols; j++)
		{
			Rectf cellRect{ g_BlockSizeX * i, g_BlockSizeY * j, g_BlockSizeX, g_BlockSizeY };
			Rectf bossCellRect{  g_BlockSizeX * i, g_BlockSizeY * j, 2 * g_BlockSizeX, 2 * g_BlockSizeY };
			if (g_MazeArray[i][j] == int(MazeEntity::wall))
				DrawTexture(g_WallTexture, cellRect);
			else if (g_MazeArray[i][j] == int(MazeEntity::path))
				DrawTexture(g_PathTexture, cellRect);
			else if (g_MazeArray[i][j] == int(MazeEntity::endPoint))
				DrawTexture(g_EndPointTexture, cellRect);
			else if (g_MazeArray[i][j] == int(MazeEntity::player1))
				DrawTexture(g_Player.texture, cellRect);
			else if (g_MazeArray[i][j] == int(MazeEntity::enemy))
			{
				if (g_Enemy1.isAlive)
					DrawTexture(g_Enemy1.texture, cellRect);
				else
				{
					g_MazeArray[i][j] = int(MazeEntity::path);
					DrawTexture(g_PathTexture, cellRect);
				}
			}
			else if (g_MazeArray[i][j] == int(MazeEntity::boss))
			{
				if (g_Boss.isAlive)
					DrawTexture(g_Boss.texture, bossCellRect);
				else
				{
					g_MazeArray[i + 1][j] = int(MazeEntity::path);
					g_MazeArray[i][j + 1] = int(MazeEntity::path);
					g_MazeArray[i + 1][j + 1] = int(MazeEntity::path);
					g_MazeArray[i][j] = int(MazeEntity::path);
					//DrawTexture(g_PathTexture, cellRect);
				}
			}
			SetColor(1.f, 1.f, 1.f);
			DrawRect(g_BlockSizeX * i, g_BlockSizeY * j, g_BlockSizeX, g_BlockSizeY);
		}
	}
}

void DrawBeams()
{
	for (int i = 0; i < g_BeamArray.size(); i++)
	{
		if(g_BeamArray[i].currDir == Direction::left || g_BeamArray[i].currDir == Direction::right)
			DrawTexture(g_BeamTexture[0], Rectf{g_BeamArray[i].x, g_BeamArray[i].y, g_BlockSizeX,g_BlockSizeY});
		else
			DrawTexture(g_BeamTexture[1], Rectf{ g_BeamArray[i].x, g_BeamArray[i].y, g_BlockSizeX,g_BlockSizeY });
	}
}

// Move entity functions
void MoveEntity(const Direction& dir, Entity& entity)
{
	bool isCollidingWithEntityX{};
	bool isCollidingWithEntityY{};
	entity.currDir = dir;
	if (dir == Direction::left || dir == Direction::right)
	{
		if (!entity.isPlayableCharacter) entity.totalMovement += int(dir);
		
		if (!entity.isBoss and entity.isPlayableCharacter) isCollidingWithEntityX = { g_MazeArray[entity.x + int(dir)][entity.y] == int(MazeEntity::enemy) or g_MazeArray[entity.x + int(dir)][entity.y] == int(MazeEntity::boss) };
		else if (!entity.isBoss and !entity.isPlayableCharacter) isCollidingWithEntityX = { g_MazeArray[entity.x + int(dir)][entity.y] == int(MazeEntity::player1) };
		else if (entity.isBoss)
		{
			if (dir == Direction::left)	isCollidingWithEntityX = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::player1) or g_MazeArray[entity.x + int(entity.currDir)][entity.y + 1] == int(MazeEntity::player1);
			else isCollidingWithEntityX = g_MazeArray[entity.x + 2 * int(entity.currDir)][entity.y] == int(MazeEntity::player1) or g_MazeArray[entity.x + 2 * int(entity.currDir)][entity.y + 1] == int(MazeEntity::player1);
		}
		if (IsMazeCellPLayerOrPath(entity) or isCollidingWithEntityX)
		{
			if (!entity.isBoss) g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
			else
			{
				if (dir == Direction::left) //redundant but fck it can't be bothered to change the statement
				{
					g_MazeArray[entity.x - int(dir)][entity.y] = int(MazeEntity::path);
					g_MazeArray[entity.x - int(dir)][entity.y + 1] = int(MazeEntity::path);
				}
				else
				{
					g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
					g_MazeArray[entity.x][entity.y + 1] = int(MazeEntity::path);
				}
			}
			entity.x += int(dir);

		}
		else if (g_MazeArray[entity.x + int(dir)][entity.y] == int(MazeEntity::endPoint) && entity.isPlayableCharacter)
		{
			//std::cout << "You won!";
			g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
			entity.x += int(dir);
			GenerateNewMaze(); // need to delete player and enemies and reload them
		}
	}
	else if (dir == Direction::up || dir == Direction::down)
	{
		if (!entity.isPlayableCharacter) entity.totalMovement += int(dir) / 2;

		if (!entity.isBoss and entity.isPlayableCharacter) isCollidingWithEntityY = { g_MazeArray[entity.x][entity.y + int(dir) / 2] == int(MazeEntity::boss) or g_MazeArray[entity.x][entity.y + int(dir) / 2] == int(MazeEntity::enemy) };
		else if (!entity.isBoss and !entity.isPlayableCharacter) isCollidingWithEntityY = { g_MazeArray[entity.x][entity.y + int(dir) / 2] == int(MazeEntity::player1) };
		else if (true)
		{
			if (dir == Direction::up) isCollidingWithEntityY = g_MazeArray[entity.x][entity.y + int(entity.currDir)] == int(MazeEntity::player1) or g_MazeArray[entity.x + 1][entity.y + int(entity.currDir)] == int(MazeEntity::player1);
			else isCollidingWithEntityY = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::player1) or g_MazeArray[entity.x + 1][entity.y + int(entity.currDir) / 2] == int(MazeEntity::player1);

		}
		if (IsMazeCellPLayerOrPath(entity) or isCollidingWithEntityY)
		{
			if (!entity.isBoss) g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
			else
			{
				if (dir == Direction::down)
				{
					g_MazeArray[entity.x][entity.y - int(dir) / 2] = int(MazeEntity::path);
					g_MazeArray[entity.x + 1][entity.y - int(dir) / 2] = int(MazeEntity::path);
				}
				else
				{
					g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
					g_MazeArray[entity.x + 1][entity.y] = int(MazeEntity::path);
				}
			}
			
			entity.y += int(dir) / 2;
		}
		else if (g_MazeArray[entity.x][entity.y + int(dir) / 2] == int(MazeEntity::endPoint) && entity.isPlayableCharacter)
		{
			//std::cout << "You won!";
			g_MazeArray[entity.x][entity.y] = int(MazeEntity::path);
			entity.y += int(dir) / 2;
			GenerateNewMaze();
		}
	}
}

bool IsMazeCellPLayerOrPath(Entity& entity)
{
	bool firstCondition{}, secondCondition{};
	switch (entity.currDir)
	{
	case Direction::up:
		if (!entity.isBoss)
		{
			firstCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::player1);
		}
		else
		{
			firstCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir)] == int(MazeEntity::path) and g_MazeArray[entity.x + 1][entity.y + int(entity.currDir)] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir)] == int(MazeEntity::player1) or g_MazeArray[entity.x + 1][entity.y + int(entity.currDir)] == int(MazeEntity::player1);
		}
		break;
	case Direction::down:
		if (!entity.isBoss)
		{
			firstCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::player1);
		}
		else
		{
			firstCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::path) and g_MazeArray[entity.x + 1][entity.y + int(entity.currDir) / 2] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x][entity.y + int(entity.currDir) / 2] == int(MazeEntity::player1) or g_MazeArray[entity.x + 1][entity.y + int(entity.currDir) / 2] == int(MazeEntity::player1);
		}
		break;
	case Direction::left:
		if (!entity.isBoss)
		{
			firstCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::player1);
		}
		else
		{
			firstCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::path) and g_MazeArray[entity.x + int(entity.currDir)][entity.y + 1] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::player1) or g_MazeArray[entity.x + int(entity.currDir)][entity.y + 1] == int(MazeEntity::player1);
		}
		break;
	case Direction::right:
		if (!entity.isBoss)
		{
			firstCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x + int(entity.currDir)][entity.y] == int(MazeEntity::player1);
		}
		else
		{
			firstCondition = g_MazeArray[entity.x + 2 * int(entity.currDir)][entity.y] == int(MazeEntity::path) and g_MazeArray[entity.x + 2 * int(entity.currDir)][entity.y + 1] == int(MazeEntity::path);
			secondCondition = g_MazeArray[entity.x + 2 * int(entity.currDir)][entity.y] == int(MazeEntity::player1) or g_MazeArray[entity.x + 2 * int(entity.currDir)][entity.y + 1] == int(MazeEntity::player1);
		}
		break;
	default:
		break;
	}

	return firstCondition or secondCondition;
}

bool IsEnemyPassedPLayer(Entity& entity)
{
	return g_Player.x == entity.x or g_Player.y == entity.y;
}

void SwitchEntityDirection(Entity& entity)
{
	if (g_Player.x > entity.x) entity.currDir = Direction::right;
	else if (g_Player.x < entity.x) entity.currDir = Direction::left;
	else if (g_Player.y > entity.y) entity.currDir = Direction::up;
	else if (g_Player.y < entity.y) entity.currDir = Direction::down;
}

bool IsDirectionCorrect(Entity& entity)
{
	if (g_Player.x > entity.x and entity.currDir == Direction::right) return true;
	else if (g_Player.x < entity.x and entity.currDir == Direction::left) return true;
	else if (g_Player.y > entity.y and entity.currDir == Direction::up) return true;
	else if (g_Player.y < entity.y and entity.currDir == Direction::down) return true;
	return false;
}

// Lost game functions
bool IsGameLost()
{
	if (g_Enemy1.isAlive) return g_Player.x == g_Enemy1.x and g_Player.y == g_Enemy1.y;
	if (g_Boss.isAlive)
	{
		bool isCollindingBossCellBottomLeft	{ g_Player.x == g_Boss.x + 0	and		g_Player.y == g_Boss.y + 0	};
		bool isCollindingBossCellBottomRight{ g_Player.x == g_Boss.x + 1	and		g_Player.y == g_Boss.y + 0	};
		bool isCollindingBossCellTopLeft	{ g_Player.x == g_Boss.x + 0	and		g_Player.y == g_Boss.y + 1	};
		bool isCollindingBossCellTopRight	{ g_Player.x == g_Boss.x + 1	and		g_Player.y == g_Boss.y + 1	};
	
		return isCollindingBossCellBottomLeft or isCollindingBossCellBottomRight or isCollindingBossCellTopLeft or isCollindingBossCellTopRight;
	}
	return false;
}

// Beams functions
bool CheckBeamCollision(const Beam& beam)
{

	Point2i beamPosInArr{ int(beam.x / g_BlockSizeX) , int(beam.y / g_BlockSizeY) };
	if(beam.currDir == Direction::right)
	{
		beamPosInArr.x = int(beam.x / g_BlockSizeX - 0.2f);
		if (g_MazeArray[beamPosInArr.x + 1][beamPosInArr.y] == int(MazeEntity::wall)) return true;
	}
	else if (beam.currDir == Direction::left)
	{
		beamPosInArr.x = int(beam.x / g_BlockSizeX + 0.2f);
		if (g_MazeArray[beamPosInArr.x ][beamPosInArr.y] == int(MazeEntity::wall)) return true;
	}
	else if (beam.currDir == Direction::up)
	{
		beamPosInArr.y = int(beam.y / g_BlockSizeY - 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y + 1] == int(MazeEntity::wall)) return true;
	}
	else
	{
		beamPosInArr.y = int(beam.y / g_BlockSizeY + 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y ] == int(MazeEntity::wall)) return true;
	}

	return false;
}

bool CheckEnemyCollision(const Beam& beam)
{

	Point2i beamPosInArr{ int(beam.x / g_BlockSizeX) , int(beam.y / g_BlockSizeY) };
	if (beam.currDir == Direction::right)
	{	
		beamPosInArr.x = int(beam.x / g_BlockSizeX - 0.2f);
		if (g_MazeArray[beamPosInArr.x + 1][beamPosInArr.y] == int(MazeEntity::enemy)) return true;
	}
	else if (beam.currDir == Direction::left)
	{
		beamPosInArr.x = int(beam.x / g_BlockSizeX + 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y] == int(MazeEntity::enemy)) return true;
	}
	else if (beam.currDir == Direction::up)
	{
		beamPosInArr.y = int(beam.y / g_BlockSizeY - 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y + 1] == int(MazeEntity::enemy)) return true;
	}
	else
	{
		beamPosInArr.y = int(beam.y / g_BlockSizeY + 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y] == int(MazeEntity::enemy)) return true;
	}

	return false;
}

bool CheckBossCollision(const Beam& beam)
{

	Point2i beamPosInArr{ int(beam.x / g_BlockSizeX) , int(beam.y / g_BlockSizeY) };
	if (beam.currDir == Direction::right)
	{
		beamPosInArr.x = int(beam.x / g_BlockSizeX - 0.2f);
		if (g_MazeArray[beamPosInArr.x + 1][beamPosInArr.y] == int(MazeEntity::boss)) return true;
	}
	else if (beam.currDir == Direction::left)
	{
		beamPosInArr.x = int(beam.x / g_BlockSizeX + 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y] == int(MazeEntity::boss)) return true;
	}
	else if (beam.currDir == Direction::up)
	{
		beamPosInArr.y = int(beam.y / g_BlockSizeY - 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y + 1] == int(MazeEntity::boss)) return true;
	}
	else
	{
		beamPosInArr.y = int(beam.y / g_BlockSizeY + 0.2f);
		if (g_MazeArray[beamPosInArr.x][beamPosInArr.y] == int(MazeEntity::boss)) return true;
	}

	return false;
}

void ClearAllBeams()
{
	for (int i = 0; i < g_BeamArray.size(); i++)
	{
		g_BeamArray.erase(g_BeamArray.begin() + i);
	}
	if (g_BeamArray.size() > 0) ClearAllBeams();
}

void DeleteBeam()
{
	for (int i = 0; i < g_BeamArray.size(); i++)
	{	
		if (CheckBeamCollision(g_BeamArray[i]) || CheckEnemyCollision(g_BeamArray[i]) or CheckBossCollision(g_BeamArray[i]))
			g_BeamArray.erase(g_BeamArray.begin() + i);
	}
}

void KillEnemy()
{
	for (int i = 0; i < g_BeamArray.size(); i++)
	{
		if (CheckEnemyCollision(g_BeamArray[i]))
		{
			g_Enemy1.isAlive = false;
		}
	}
}

void SpawnLadder()
{
	if (g_MazeArray[g_NrOfRows / 2][g_NrOfCols / 2] == int(MazeEntity::path)) g_MazeArray[g_NrOfRows / 2][g_NrOfCols / 2] = int(MazeEntity::endPoint);
	else if (g_MazeArray[g_NrOfRows / 2][g_NrOfCols / 2 - 5] == int(MazeEntity::path)) g_MazeArray[g_NrOfRows / 2][g_NrOfCols / 2 - 5] = int(MazeEntity::endPoint);
	else
	{
		int pos = rand() % (g_NrOfRows - 2);
		if (g_MazeArray[pos][pos] == int(MazeEntity::path))	g_MazeArray[pos][pos] = int(MazeEntity::endPoint);
		else SpawnLadder();
	}
}

void KillBoss()
{
	for (int i = 0; i < g_BeamArray.size(); i++)
	{
		if (CheckBossCollision(g_BeamArray[i]))
		{
			g_Boss.health -= 1;
			if (g_Boss.health == 0)
			{
				g_Boss.isAlive = false;
				SpawnLadder();
			}
		}
	}
}

#pragma endregion ownDefinitions