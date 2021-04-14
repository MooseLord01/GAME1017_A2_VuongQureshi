#include "States.h"
#include "Engine.h"
#include "EventManager.h"
#include "TextureManager.h"

using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume() {}

