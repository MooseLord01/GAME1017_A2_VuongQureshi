#include "Engine.h"
#include "StateManager.h"
#include "States.h"
#include <iostream>
#include <ctime>


#include "EventManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "TitleState.h"

using namespace std;

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	m_bRunning = false;

	cout << "Initializing game." << endl;
	srand((unsigned)time(NULL));
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				std::cout << "Renderer Initialization Successful!" << std::endl;
				// Init subsystems/managers.
				TextureManager::Init();
				EventManager::Init();
				SoundManager::Init();

			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	STMA::PushState(new TitleState());
	m_bRunning = true; // Everything is okay, start the engine.
	cout << "Initialization Successful!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	EventManager::HandleEvents();
}

void Engine::Update()
{
	STMA::Update();
}

void Engine::Render()
{
	STMA::Render();
}

void Engine::Clean()
{
	cout << "Cleaning Engine..." << endl;
	TextureManager::Quit();
	EventManager::Quit();
	SoundManager::Quit();
	StateManager::Quit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (Init("GAME1017 Assignment 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_bRunning)
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_bRunning)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance;
	return instance;
}

bool& Engine::Running() { return m_bRunning; } // Getter.
void Engine::SetRunning(const bool b) { m_bRunning = b; }