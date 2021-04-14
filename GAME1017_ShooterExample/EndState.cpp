#include "EndState.h"

#include <iostream>

#include "Engine.h"
#include "SoundManager.h"
#include "TextureManager.h"


EndState::EndState(bool win)
{
	m_gameWon = win;
}

void EndState::Enter()
{
	cout << "Entering EndState..." << endl;

	m_pBkg = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bkgNoCircles.png");
	m_pBkgB = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bkgCirclesV.png");
	TextureManager::Load("Img/restart1.png", "restartU");
	TextureManager::Load("Img/restart2.png", "restartO");
	TextureManager::Load("Img/restart3.png", "restartD");
	TextureManager::Load("Img/quit1.png", "quitU");
	TextureManager::Load("Img/quit2.png", "quitO");
	TextureManager::Load("Img/quit3.png", "quitD");

	SOMA::AllocateChannels(10);
	SoundManager::Load("Aud/softding.mp3", "ding", SOUND_SFX);
	if(!m_gameWon)
	{
		SoundManager::Load("Aud/niercityruins.mp3", "lose", SOUND_MUSIC);
		SoundManager::SetMusicVolume(15);
		SoundManager::PlayMusic("lose", -1, 0);
		m_pEndLoseText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/gameOver.png");
		m_endLose = { {0,0,1024,768}, {0, 0, 1024, 768} };
	}
	else
	{
		SoundManager::Load("Aud/ohBanana.mp3", "oh", SOUND_SFX);
		SOMA::SetSoundVolume(5);
		SOMA::PlaySound("oh", 0, -1);

		SoundManager::Load("Aud/win.mp3", "win", SOUND_MUSIC);
		SoundManager::SetMusicVolume(10);
		SoundManager::PlayMusic("win", -1, 0);
		m_pEndWinText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/winner.png");
		m_endWin = { {0,0,1024,768}, {0, 0, 1024, 768} };
	}
	
	
	m_restartBtn = new RestartButton({ 0,0,265,58 }, { WIDTH / 2 - 130,500,265,58 }, "restartU", "restartO", "restartD");
	m_quitBtn = new QuitButton({ 0,0,265,58 }, { WIDTH / 2 - 130,585,265,58 }, "quitU", "quitO", "quitD");
	m_endBG = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_endBGB = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_endBGArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_endBGArray[1] = { {0,0,1024,768}, {0, 768, 1024, 768} };
	m_endBGBArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_endBGBArray[1] = { {0,0,1024,768}, {0, 768, 1024, 768} };
}

void EndState::Update()
{
	m_quitBtn->Update();
	m_restartBtn->Update();
	
	// Background scrolling. Vertical, so y.
	for (int i = 0; i < 2; i++)
		m_endBGArray[i].GetDstP()->y -= BGSCROLL / 2;
	if (m_endBGArray[1].GetDstP()->y <= 0)
	{
		m_endBGArray[0].GetDstP()->y = 0;
		m_endBGArray[1].GetDstP()->y = 768;
	}

	for (int i = 0; i < 2; i++)
		m_endBGBArray[i].GetDstP()->y -= BGSCROLL;
	if (m_endBGBArray[1].GetDstP()->y <= 0)
	{
		m_endBGBArray[0].GetDstP()->y = 0;
		m_endBGBArray[1].GetDstP()->y = 768;
	}

}

void EndState::Render()
{
	State::Render();

	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer()); // Clear the screen with the draw color.

	// Render stuff. Background first.
	for (int i = 0; i < 2; i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBkg, m_endBGArray[i].GetSrcP(), m_endBGArray[i].GetDstP());
	for (int i = 0; i < 2; i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBkgB, m_endBGBArray[i].GetSrcP(), m_endBGBArray[i].GetDstP());

	// If the game is not won, render the Lose header.
	if(!m_gameWon)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pEndLoseText, m_endLose.GetSrcP(), m_endLose.GetDstP());
	// Else, if the game is won, render the Win header.
	else
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pEndWinText, m_endWin.GetSrcP(), m_endWin.GetDstP());
	
	m_quitBtn->Render();
	m_restartBtn->Render();

	SDL_RenderPresent(Engine::Instance().GetRenderer());

}

void EndState::Exit()
{
	cout << "Exiting GameState..." << endl;

	delete m_restartBtn;
	m_restartBtn = nullptr;
	delete m_quitBtn;
	m_quitBtn = nullptr;
		SDL_DestroyTexture(m_pBkg);
	
	if(!m_gameWon)
		SDL_DestroyTexture(m_pEndLoseText);
	else
		SDL_DestroyTexture(m_pEndWinText);
	
	SOMA::StopMusic();
}
