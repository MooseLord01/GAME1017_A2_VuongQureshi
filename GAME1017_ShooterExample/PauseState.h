#pragma once
#ifndef _PAUSESTATE_H_
#define _PAUSESTATE_H_

#include <SDL.h>

#include "Sprites.h"
#include "States.h"


class PauseState : public State
{
private:
	ResumeButton* m_resumeBtn;
	SDL_Texture* m_pPause;
	Sprite m_pauseImg;
	bool m_renderCheck;
public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

#endif
