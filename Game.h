#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "Project name - Name, firstname - 1DAExx" };

// Change the window dimensions here
float g_WindowWidth{ 500 };
float g_WindowHeight{ 500 };
#pragma endregion gameInformation



#pragma region ownDeclarations
// Declare your own global variables here
const int g_NrOfRows{ 12 }, g_NrOfCols{ 12 };
int g_MazeArray[g_NrOfRows][g_NrOfCols]{};
const float g_BlockSize{ 42.f };

enum class Direction {
	up = 1,
	left = -1,
	right = 1,
	down = -1
}; Direction g_CurrDir{};

struct Player {
	int x;
	int y;
	Texture texture;
};
Player g_Player1{ 1,1 };
// Declare your own functions here
void InitializeMaze();
void DrawMaze();
void UpdatePlayerPos();
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
