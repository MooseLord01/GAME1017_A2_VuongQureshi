#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include "Sprites.h"

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define BGSCROLL 2
#define PSPEED 6

class Engine
{
private: // Private properties.
	bool m_bRunning;
	const Uint8* m_iKeystates;				// Keyboard state container.
	Uint32 m_start, m_end, m_delta, m_fps;  // Fixed timestep variables.
	SDL_Window* m_pWindow;					// This represents the SDL window.
	SDL_Renderer* m_pRenderer;				// This represents the buffer to draw to.

private: // Private methods.
	Engine() {}
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void Wake();
	void Sleep();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
public: // Public methods.
	int Run();
	static Engine& Instance();
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	SDL_Window* GetWindow() { return m_pWindow; }
	TTF_Font* getFont() const;
	bool& Running();
	void SetRunning(const bool b);
};

#endif
