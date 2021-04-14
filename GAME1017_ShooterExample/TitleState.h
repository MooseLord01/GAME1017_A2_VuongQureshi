#pragma once
#ifndef _TITLESTATE_H_
#define _TITLESTATE_H_
#include <SDL.h>


#include "Sprites.h"
#include "States.h"


class TitleState : public State
{
private:
	SDL_Texture* m_pTitleBG;
	SDL_Texture* m_pTitleBGB;
	SDL_Texture* m_pTitle;
	Sprite m_titleBG;
	Sprite m_titleBGArray[2];
	Sprite m_titleBGB;
	Sprite m_titleBGBArray[2];
	Sprite m_title;
	Button* m_startBtn;
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};
#endif
