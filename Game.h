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
int g_NrOfRows{ 26 }, g_NrOfCols{ 26 };
int** g_MazeArray = new int* [g_NrOfRows];
const int g_MaxNrOfBeams{ 50 };

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
enum class MazeEntity
{
	path = 1,
	wall = 0,
	player1 = 3,
	endPoint = 2,
	enemy = 4,
	boss = 5,
}; 

struct Entity {
	int x;
	int y;
	int health;
	Texture texture;
	Direction currDir{ Direction::right };
	int totalMovement;
	bool isBoss;
	bool isFollowing;
	bool isPlayableCharacter;
	bool isAlive{ true };
	bool isEnemySpawner{};
};
Entity g_Player{ }, g_Enemy1{}, g_Boss{};
struct Beam {
	float x;
	float y;
	Direction currDir;
	Beam(float x_pos, float y_pos) : x(x_pos), y(y_pos), currDir(Direction::right) {};
	
}; 
std::vector<Beam> g_BeamArray{};
std::vector<Entity> g_EnemyArray{};
std::vector<Entity> g_SpawnerArray{};

Texture g_StartPage{}, g_StartButton{}, g_LostGamePage{}, g_RetryButton{}, g_BeamTexture[2]{};
Texture g_WallTexture{}, g_PathTexture{}, g_EndPointTexture{}, g_LevelTexture{}, g_InfoPanel{};
Rectf g_ButtonRect{} ;
bool g_IsGameStarted{ false }, g_IsInMenu{ false };
int g_LevelNr{ 1 };
// Declare your own functions here
void InitializeGameResources(int playerStartPosX, int playerStartPosY);
void InitializeMaze();
void DrawMaze();
void DrawBeams();
void UpdatePlayerPos(const Entity& player);
void UpdateEnemyPos(const int movement, Entity& entity);
void UpdateLevel(int level);
void UpdateTime();
void UpdateBeam();
void MoveEntity(const Direction& dir, Entity& entity);
bool IsMazeCellPLayerOrPath(Entity& entity);
bool IsEnemyPassedPLayer(Entity& entity);
void SwitchEntityDirection(Entity& entity);
bool IsDirectionCorrect(Entity& entity);
void DeleteBeam();
void GenerateNewMaze();
void Display2DArray();
void DepthFirstSearch(int x, int y);
bool isValidPos(int x, int y, int rows, int cols);
std::vector<Point2i> getAdjacentArray(int x, int y);
bool IsGameLost();
void DeleteTextures();
void InitializeTextures();
void ClearEnemies();
void ClearAllBeams();
void KillEnemy();
void KillBoss();
void SpawnLadder();
bool CheckEnemyCollision(const Beam& beam);
bool CheckBossCollision(const Beam& beam);
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
