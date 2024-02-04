#pragma once
#include <iostream>
#include "vendors/sdl/SDL.h"
struct Vector4
{
	int r;
	int g;
	int b;
	int a;
};
struct Vector2
{
	float x;
	float y;
};
class Game
{
public:
	Game();

	// Initialize the Game
	bool Initialize();

	// Runs the game loop until the game is over
	void RunLoop();

	// Shutdown the game
	void Shutdown();

private:

	// Helper Functions for the game loop
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	// Window Created by SDL
	SDL_Window* mWindow;

	// SDL Renderer
	SDL_Renderer* mRenderer;

	// Position Vectors
	Vector2 mPaddlePos, mPaddle2Pos, mBallPos, mBallVel;

	Vector4 mBackgroundColor, mObjectColor;

	Uint32 mTicksCount;

	int mPaddleDir, mPaddle2Dir;;

	// Game should continue to run
	bool mIsRunning;
};