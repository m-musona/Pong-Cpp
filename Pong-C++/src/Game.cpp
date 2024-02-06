#pragma once
#include "Game.h"

const int thickness = 15;
const float paddleHeight = 100.0f;
const float screenWidth = 1024.0f;
const float screenHeight = 720.0f;
const float ballspeedmultiplier = 0.1f;


Game::Game()
	: mWindow(nullptr), mIsRunning(true), mTicksCount(0), mPaddleDir(0), mPaddle2Dir(0)
{
}

bool Game::Initialize()
{
	// Iniotialize SDL library
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	// If sdl initialization fails return false
	if (sdlResult != 0)
	{
		// SDL Log is a way to output things to the console in SDL
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	// Create window
	mWindow = SDL_CreateWindow(
		"Pong C++", // Window Title
		0, // Top left x-coordinate of window
		0, // Top left y-coordinate of window
		static_cast<int>(screenWidth), // Width of window
		static_cast<int>(screenHeight), // Height of window
		0 // Flags (0 for no flags set)
	);
	// Verify that window created succsefully
	if (!mWindow)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	// Set Background color to blue and objects to white
	mBackgroundColor = { 0, 0, 255, 255 };
	mObjectColor = { 255, 255, 255, 255 };


	// Create renderer after creating window
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create the renderer for
		-1, // Which Graphics driver to use (-1 is let SDL decide)
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Flags (1st one is take advantage of graphics hardware 2nd one is vsync on)
	);

	mPaddlePos.x = 10.0f;
	mPaddlePos.y = screenHeight / 2.0f;
	mPaddle2Pos.x = screenWidth - 10.0f;
	mPaddle2Pos.y = screenHeight / 2.0f;
	mBallPos.x = screenWidth / 2.0f;
	mBallPos.y = screenHeight / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	// Shutdown SDL Functions
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	// While there still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// Handle different event types here

		case SDL_QUIT:
			mIsRunning = false;

			break;
		}
	}

	// Get State of Keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Reset paddle direction
	mPaddleDir = 0;
	mPaddle2Dir = 0;
	// If W is pressed set paddle direction to mPaddleDir - 1
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}

	// If S is pressed set paddle direction to mPaddleDir + 1
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	// If Up is pressed set paddle direction to mPaddle2Dir - 1
	if (state[SDL_SCANCODE_UP])
	{
		mPaddle2Dir -= 1;
	}

	// If Down is pressed set paddle direction to mPaddle2Dir + 1
	if (state[SDL_SCANCODE_DOWN])
	{
		mPaddle2Dir += 1;
	}

	// Change Color on pressing M, N, B, V or C
	changeBgColrObjColr(state[SDL_SCANCODE_M], { 189, 123, 157, 255 }, { 255, 255, 255, 255 });
	
	changeBgColrObjColr(state[SDL_SCANCODE_N], { 0, 0, 0, 255 }, { 255, 255, 255, 255 });

	changeBgColrObjColr(state[SDL_SCANCODE_B], { 113, 203, 38, 255 }, { 255, 255, 255, 255 });

	changeBgColrObjColr(state[SDL_SCANCODE_V], { 255, 123, 0, 255 }, { 255, 255, 255, 255 });

	changeBgColrObjColr(state[SDL_SCANCODE_C], { 0, 0, 255, 255 }, { 255, 255, 255, 255 });

}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from the last frame (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update objects in the game world as function of delta time;

	// Update paddle based on delta time
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		// Make sure paddle doesn't move off Screen
		if (mPaddlePos.y < (paddleHeight / 2.0f + thickness))
		{
			mPaddlePos.y = paddleHeight / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (screenHeight - paddleHeight / 2.0f - thickness))
		{
			mPaddlePos.y = screenHeight - paddleHeight / 2.0f - thickness;
		}

	}
	// Update paddle2 based on delta time
	if (mPaddle2Dir != 0)
	{
		mPaddle2Pos.y += mPaddle2Dir * 300.0f * deltaTime;

		// Make sure paddle 2 doesn't move off Screen
		if (mPaddle2Pos.y < (paddleHeight / 2.0f + thickness))
		{
			mPaddle2Pos.y = paddleHeight / 2.0f + thickness;
		}
		else if (mPaddle2Pos.y > (screenHeight - paddleHeight / 2.0f - thickness))
		{
			mPaddle2Pos.y = screenHeight - paddleHeight / 2.0f - thickness;
		}

	}

	// Update position of ball
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// If ball bounces on top wall
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}

	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;
	float diff2 = mPaddle2Pos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;

	// if ball collides with paddle
	if (diff <= paddleHeight / 2.0f && // Our y-difference is small enough
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f && // Ball is at the correct x-position
		mBallVel.x < 0.0f) // The ball is moving to the left
	{
		mBallVel.x *= -1.0f - ballspeedmultiplier;
	}
	// if ball collides with paddle 2
	if (diff2 <= paddleHeight / 2.0f && // Our y-difference is small enough
		mBallPos.x <= screenWidth && mBallPos.x >= screenWidth - 10.0f && // Ball is at the correct x-position
		mBallVel.x > 0.0f) // The ball is moving to the Right
	{
		mBallVel.x *= -1.0f + ballspeedmultiplier;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		std::cout << "Game Ended Right Player Wins" << std::endl;
		mIsRunning = false;
	}
	else if (mBallPos.x >= screenWidth)
	{
		std::cout << "Game Ended Left Player Wins" << std::endl;
		mIsRunning = false;
	}
	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1 + ballspeedmultiplier;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (static_cast<int>(screenHeight) - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1 - ballspeedmultiplier;
	}
}
void Game::GenerateOutput()
{
	// Step 1: Clear the back buffer to a color
	SDL_SetRenderDrawColor(mRenderer, 
		mBackgroundColor.r /* R */ ,
		mBackgroundColor.g /* G */ ,
		mBackgroundColor.b /* B */ ,
		mBackgroundColor.a /* A */ );
	// Clear back buffer to current draw color
	SDL_RenderClear(mRenderer);

	// Step 2: Draw the entire game scene
	// Drawing the Rectangles on the screen
	SDL_SetRenderDrawColor(mRenderer, mObjectColor.r, mObjectColor.g, mObjectColor.b, mObjectColor.a);

	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		static_cast<int>(screenWidth),		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = static_cast<int>(screenHeight) - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleHeight / 2),
		thickness,
		static_cast<int>(paddleHeight)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// Draw paddle2
	SDL_Rect paddle2{
		static_cast<int>(mPaddle2Pos.x),
		static_cast<int>(mPaddle2Pos.y - paddleHeight / 2),
		thickness,
		static_cast<int>(paddleHeight)
	};
	SDL_RenderFillRect(mRenderer, &paddle2);

	// Draw ball
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	//Step 3: Swap the front buffer and back buffer
	SDL_RenderPresent(mRenderer);


}

void Game::changeBgColrObjColr(Uint8 KeybordState, Vector4 bgColr, Vector4 objColr)
{
	if (KeybordState)
	{
		mBackgroundColor = bgColr;
		mObjectColor = objColr;
	}
}
