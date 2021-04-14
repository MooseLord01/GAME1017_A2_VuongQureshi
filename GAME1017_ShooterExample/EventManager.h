#pragma once
#ifndef _EVENTMANAGER_H_
#define _EVENTMANAGER_H_

#include <SDL.h>

class EventManager
{
public:
	static void Init();
	static void HandleEvents();

	static bool KeyHeld(const SDL_Scancode k);
	static bool KeyPressed(const SDL_Scancode k);
	static bool KeyReleased(const SDL_Scancode k);

	static int LastKeyDown();
	static int LastKeyUp();
	
	static bool MouseHeld(const int b);
	static bool MousePressed(const int b);
	static bool MouseReleased(const int b);
	static SDL_Point& GetMousePos();
	static void SetCursor(const SDL_SystemCursor& cursor);

	static void Quit();
private:

	static const Uint8* s_keysCurrent;
	static Uint8* s_keysLast;
	static int s_numKeys;
	static int s_lastKeyDown;
	static int s_lastKeyUp;
	
	static SDL_Point s_mousePosition;
	static Uint32 s_mouseCurrent;
	static Uint32 s_mouseLast;
	static SDL_Cursor* s_cursor;

	EventManager() {}
	
};

typedef EventManager EVMA;

#endif
