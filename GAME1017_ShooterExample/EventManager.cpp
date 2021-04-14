#include "EventManager.h"
#include "Engine.h"

#include <cstring>
#include <iostream>


void EventManager::Init()
{
	s_keysCurrent = SDL_GetKeyboardState(&s_numKeys);
	s_keysLast = new Uint8[s_numKeys];
	std::memcpy(s_keysLast, s_keysCurrent, s_numKeys);
	s_mouseCurrent = SDL_GetMouseState(&s_mousePosition.x, &s_mousePosition.y);
	s_mouseLast = s_mouseCurrent;
	std::cout << "EventManager initialization complete!" << std::endl;
}

void EventManager::HandleEvents()
{
	SDL_Event event;

	std::memcpy(s_keysLast, s_keysCurrent, s_numKeys);
	s_mouseLast = s_mouseCurrent;
	s_lastKeyUp = s_lastKeyDown = -1;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			Engine::Instance().Running() = false;
			break;
		case SDL_KEYDOWN:
			s_lastKeyDown = event.key.keysym.sym;
			break;
		case SDL_KEYUP:
			s_lastKeyUp = event.key.keysym.sym;
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				Engine::Instance().SetRunning(false);
			}
			break;
		}
	}
	s_keysCurrent = SDL_GetKeyboardState((&s_numKeys));
	s_mouseCurrent = SDL_GetMouseState(&s_mousePosition.x, &s_mousePosition.y);
}

bool EventManager::KeyHeld(const SDL_Scancode k)
{
	if(s_keysCurrent != nullptr)
	{
		if (s_keysCurrent[k] == 1)
			return true;
		else
			return false;
	}
	return false;
}

bool EventManager::KeyPressed(const SDL_Scancode k)
{
	return s_keysCurrent[k] > s_keysLast[k];
}

bool EventManager::KeyReleased(const SDL_Scancode k)
{
	return s_keysCurrent[k] < s_keysLast[k];
}

int EventManager::LastKeyDown()
{
	return s_lastKeyDown;
}

int EventManager::LastKeyUp()
{
	return s_lastKeyUp;
}

bool EventManager::MouseHeld(const int b)
{
	if (b >= 1 && b <= 3)
		return (s_mouseCurrent & SDL_BUTTON(b));
	else
		return false;
}

bool EventManager::MousePressed(const int b)
{
	return ((s_mouseCurrent & SDL_BUTTON(b)) > (s_mouseLast & SDL_BUTTON(b)));
}

bool EventManager::MouseReleased(const int b)
{
	return ((s_mouseCurrent & SDL_BUTTON(b)) < (s_mouseLast & SDL_BUTTON(b)));
}

SDL_Point& EventManager::GetMousePos()
{
	return s_mousePosition;
}

void EventManager::SetCursor(const SDL_SystemCursor& cursor)
{
	SDL_FreeCursor((s_cursor));
	s_cursor = SDL_CreateSystemCursor(cursor);
	SDL_SetCursor(s_cursor);
}

void EventManager::Quit()
{
	delete s_keysLast;
	SDL_FreeCursor(s_cursor);
}

const Uint8* EventManager::s_keysCurrent = nullptr;
Uint8* EventManager::s_keysLast;
int EventManager::s_numKeys;

int EventManager::s_lastKeyDown;
int EventManager::s_lastKeyUp;
SDL_Point EventManager::s_mousePosition = { 0,0 };
Uint32 EventManager::s_mouseCurrent;
Uint32 EventManager::s_mouseLast;

SDL_Cursor* EventManager::s_cursor;