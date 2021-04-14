#include "TitleState.h"

#include <iostream>
#include "Engine.h"
#include "GameState.h"
#include "SoundManager.h"
#include "TextureManager.h"


TitleState::TitleState() {}

void TitleState::Enter()
{
	cout << "Entering TitleState..." << endl;

	m_pTitleBG = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bkgNoCircles.png");
	m_pTitleBGB = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bkgCirclesV.png");
	m_pTitle = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/title.png");
	TextureManager::Load("Img/start1.png", "startU");
	TextureManager::Load("Img/start2.png", "startO");
	TextureManager::Load("Img/start3.png", "startD");

	SOMA::AllocateChannels(2);
	SoundManager::Load("Aud/softding.mp3", "ding", SOUND_SFX);
	SoundManager::Load("Aud/niercityruins.mp3", "titleBkg", SOUND_MUSIC);
	SoundManager::SetMusicVolume(15);
	SoundManager::PlayMusic("titleBkg", -1, 0);

	m_startBtn = new Button({ 0,0,265,58 }, { WIDTH / 2 - 130,500,265,58 }, "startU", "startO", "startD");
	m_title = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_titleBGArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_titleBGArray[1] = { {0,0,1024,768}, {0, 768, 1024, 768} };
	m_titleBGBArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_titleBGBArray[1] = { {0,0,1024,768}, {0, 768, 1024, 768} };
}

void TitleState::Update()
{
	m_startBtn->Update();
	// Background scrolling. Vertical, so y.
	for (int i = 0; i < 2; i++)
		m_titleBGArray[i].GetDstP()->y -= BGSCROLL / 2;
	if (m_titleBGArray[1].GetDstP()->y <= 0)
	{							   
		m_titleBGArray[0].GetDstP()->y = 0;
		m_titleBGArray[1].GetDstP()->y = 768;
	}
	for (int i = 0; i < 2; i++)
		m_titleBGBArray[i].GetDstP()->y -= BGSCROLL;
	if (m_titleBGBArray[1].GetDstP()->y <= 0)
	{							   
		m_titleBGBArray[0].GetDstP()->y = 0;
		m_titleBGBArray[1].GetDstP()->y = 768;
	}
}

void TitleState::Render()
{
	cout << "Rendering TitleState..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	for (int i = 0; i < 2; i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pTitleBG, m_titleBGArray[i].GetSrcP(), m_titleBGArray[i].GetDstP());
	
	for (int i = 0; i < 2; i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pTitleBGB, m_titleBGBArray[i].GetSrcP(), m_titleBGBArray[i].GetDstP());

	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pTitle, m_title.GetSrcP(), m_title.GetDstP());
	m_startBtn->Render();

	State::Render();
}

void TitleState::Exit()
{
	SoundManager::PlaySound("ding", 0, 2);
	SOMA::StopMusic();
	cout << "Exiting TitleState..." << endl;
}
// End TitleState
