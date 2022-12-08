#pragma once
#include <chrono>
#include <vector>
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "Project name - Name, firstname - 1DAExx" };

// Change the window dimensions here
float g_WindowHeight{ 720 };
float g_WindowWidth{ g_WindowHeight }; //mofifiable width if we have more columns?
#pragma endregion gameInformation

	

#pragma region ownDeclarations
// Maze variables
int g_NrOfRows{ 25 }, g_NrOfCols{ 25 };
int** g_MazeArray = new int* [g_NrOfRows];
////int g_MazeArray[g_NrOfRows][g_NrOfCols]{};
float g_BlockSizeX{ g_WindowWidth/ g_NrOfRows };
float g_BlockSizeY{ g_WindowHeight/g_NrOfCols };

// Enemy UpdateTime
int g_Nrframes{ 1 };
float g_TotalTimePassed{};
std::chrono::time_point<std::chrono::system_clock> g_TimeStart{}, g_TimeEnd{};
std::chrono::duration<float> g_DeltaTime;

enum class Direction {
	up = 2,
	left = -1,
	right = 1,
	down = -2
}; 
enum class MazeEntity {
	path = 1,
	wall = 0,
	player1 = 3,
	endPoint = 2,
	enemy = 4
}; 
struct Entity {
	int x;
	int y;
	Texture texture;
	Direction currDir;
	int totalMovement;
	bool isFollowing;
	bool isPlayableCharacter; //not in use yet
	bool isAlive{ true };
};
Entity g_Player1{ }, g_Enemy1{};

Texture g_StartPage{}, g_StartButton{}, g_LostGamePage{}, g_RetryButton{};
Texture g_WallTexture{}, g_PathTexture{}, g_EndPointTexture{};
Rectf g_ButtonRect{} ;
bool g_IsGameStarted{ false };

// Declare your own functions here
void InitializeGameResources(int playerStartPosX, int playerStartPosY, int enemyStartPosX, int enemyStartPosY);
void InitializeMaze();
void DrawMaze();
void UpdatePlayerPos(const Entity& player);
void UpdateEnemyPos(const int movement, Entity& entity);
void UpdateTime();
void MoveEntity(const Direction& dir, Entity& entity);
bool IsMazeCellPLayerOrPath(Entity& entity);
bool IsEnemyPassedPLayer(Entity& entity);
void SwitchEntityDirection(Entity& entity);
bool IsDirectionCorrect(Entity& entity);

void GenerateNewMaze();
void Display2DArray();
void DepthFirstSearch(int x, int y);
bool isValidPos(int x, int y, int rows, int cols);
std::vector<Point2i> getAdjacentArray(int x, int y);
bool IsGameLost();
void DeleteTextures();
void InitializeTextures();
void ClearEnemies();
#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
