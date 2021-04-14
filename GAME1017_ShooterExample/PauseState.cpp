#include "PauseState.h"

#include <iostream>
#include "Engine.h"
#include "EventManager.h"
#include "StateManager.h"
#include "TextureManager.h"

// Begin PauseState  --------------------------------------------------------------------------
PauseState::PauseState() {}

void PauseState::Enter()
{
	cout << "Entering Pausestate..." << endl;

	TextureManager::Load("Img/resume1.png", "resumeU");
	TextureManager::Load("Img/resume2.png", "resumeO");
	TextureManager::Load("Img/resume3.png", "resumeD");
	
	m_pPause = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/pauseImg.png");
	m_pauseImg = { {0,0,690,244}, {180,150,690,244} };

	m_resumeBtn = new ResumeButton({ 0,0,265,58 }, { WIDTH / 2 - 130,500,265,58 }, "resumeU", "resumeO", "resumeD");
}

void PauseState::Update()
{
	if (EventManager::KeyPressed(SDL_SCANCODE_R))
	{
		STMA::PopState();
		return;
	}

	m_resumeBtn->Update();
	
}

void PauseState::Render()
{

	// Render GameState
	STMA::GetStates().front()->Render();

	// So the pause state doesn't continuously render on top of itself. Just once.
	if(m_renderCheck == false)
	{
		m_renderCheck = true;
		// Render rest of PauseState
		SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
		//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 155, 221, 64);
		SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 20, 20, 20, 60);
		//SDL_Rect rect = { 256, 128, 512, 512 };
		SDL_Rect rect = { 50, 50, WIDTH-100, HEIGHT-100 };
		SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pPause, m_pauseImg.GetSrcP(), m_pauseImg.GetDstP());
	}
	State::Render();
	m_resumeBtn->Render();
}

void PauseState::Exit()
{
	m_renderCheck = false;
	cout << "Exiting PauseState..." << endl;
	TextureManager::Unload("resumeU");
	TextureManager::Unload("resumeO");
	TextureManager::Unload("resumeD");
}
// End PauseState

