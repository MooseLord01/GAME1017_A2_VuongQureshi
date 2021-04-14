#pragma once
#ifndef _ENDSTATE_H_
#define _ENDSTATE_H_
#include <SDL.h>


#include "Sprites.h"
#include "States.h"

class EndState : public State
{
private:
	SDL_Texture* m_pEndLoseText;
	SDL_Texture* m_pEndWinText;
	SDL_Texture* m_pBkg;
	SDL_Texture* m_pBkgB;
	Sprite m_endBG;
	Sprite m_endBGArray[2];
	Sprite m_endBGB;
	Sprite m_endBGBArray[2];
	Sprite m_endLose;
	Sprite m_endWin;
	RestartButton* m_restartBtn;
	QuitButton* m_quitBtn;

	// If the game was won. So we can render the appropriate header.
	bool m_gameWon;

public:
	EndState(bool win);
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

#endif
