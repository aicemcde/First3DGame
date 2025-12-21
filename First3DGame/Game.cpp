#include "Game.h"
#include "VertexArray.h"
#include "Shader.h"
#include <cstdint>
#include "Scene.h"
#include "Actor.h"
#include "ResourceManager.h"
#include <SDL_ttf.h>
#include "Renderer.h"


Game* Game::sInstance = nullptr;



Game::Game()
	:mWindow(nullptr)
	,mIsRunning(true)
	,mContext(nullptr)
	,mTicksCount(0)
	,mScreenSize(1024.0f, 768.0f)
{
	if (sInstance == nullptr)sInstance = this;

}

Game::~Game()
{

}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("SDL could not initialize! SDL_Error : %s", SDL_GetError());
		return false;
	}

	//ランタイムチェックはCreateWindowでできるので、SetAttributeでする必要はない
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	mWindow = SDL_CreateWindow("Asteroids_OpenGL", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("SDL cannot create Window! SDL_Error : %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() != 0)
	{
		SDL_Log("TTF could not initialize!");
		return false;
	}

	mContext = SDL_GL_CreateContext(mWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW could not initialize!");
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		SDL_Log("Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
	}

	glGetError();

	mScene = std::make_unique<Scene>();
	mResourceManager = std::make_unique<ResourceManager>();
	mRenderer = std::make_unique<Renderer>(this);
	if (!mRenderer->Initialize())
	{
		SDL_Log("Renderer could not initialize!");
		return false;
	}

	LoadData();

	return true;
}

void Game::Shutdown()
{
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
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

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		default:
			break;

		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const uint8_t* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mScene->ProcessInput(keyState);

}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mScene->Update(deltaTime);
	//ColorfulBG(deltaTime);
}

void Game::GenerateOutput()
{
	mRenderer->Draw();

	SDL_GL_SwapWindow(mWindow);
}

void Game::LoadData()
{
	
}

void Game::UnloadData()
{
	mRenderer->UnloadData();
}

void Game::ColorfulBG(float deltaTime)
{
	if (mRGB_bgColor.z < 1.0f)
	{
		mRGB_bgColor.z += deltaTime / 5.0f;
		if (mRGB_bgColor.z > 1.0f)
		{
			mRGB_bgColor.z = 1.0f;
		}
	}
	glClearColor(mRGB_bgColor.x, mRGB_bgColor.y, mRGB_bgColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}