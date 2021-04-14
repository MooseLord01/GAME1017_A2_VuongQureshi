#include "GameState.h"

#include <iostream>

#include "EndState.h"
#include "Engine.h"
#include "EventManager.h"
#include "PauseState.h"
#include "TitleState.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "Utilities.h"

// Begin GameState  --------------------------------------------------------------------------
GameState::GameState() {}

void GameState::Enter() // Used for initialization.
{
	cout << "Entering GameState..." << endl;
	
	m_iESpawn = 0;
	m_iESpawnMax = 60;
	m_pivot = { 0,0 };
	m_bEBNull = m_bENull = m_bPBNull = false; // Setting all to false.
	m_bCanShoot = true;

	m_pBGText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bkgNoCircles.png");
	m_pBGBText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bkgCirclesH.png");
	m_pSprText = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/sprites.png");

	SOMA::AllocateChannels(15);
	SOMA::Load("Aud/xivweightoftheworld.mp3", "bkg", SOUND_MUSIC);
	SOMA::Load("Aud/pop1.mp3", "pop1", SOUND_SFX);
	SOMA::Load("Aud/pop2.mp3", "pop2", SOUND_SFX);
	SOMA::Load("Aud/boom1.mp3", "boom1", SOUND_SFX);
	SOMA::Load("Aud/smack.mp3", "smack", SOUND_SFX);

	m_gameBGArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_gameBGArray[1] = { {0,0,1024,768}, {1024, 0, 1024, 768} };
	m_gameBGBArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	m_gameBGBArray[1] = { {0,0,1024,768}, {1024, 0, 1024, 768} };
	m_player = new Player({ 0,0,83,100 }, { 256,384 - 50,83,100 });

	SOMA::PlayMusic("bkg", -1, 0);
	SOMA::SetMusicVolume(10);
	SOMA::SetSoundVolume(15);

	m_score = 0;
}

void GameState::Update()
{
	// Change to TitleState.
	if (EventManager::KeyPressed(SDL_SCANCODE_X))
	{
		Exit();
		STMA::ChangeState(new TitleState());
		return;
	}
	// If the player died, go to the end state (lose version).
	if(m_isDead)
	{
		Exit();
		STMA::ChangeState(new EndState(false));
		return;
	}
	// If the player killed 15 enemies, go to the end state (win version).
	if(m_win)
	{
		Exit();
		STMA::ChangeState(new EndState(true));
		return;
	}
	// Invoke PauseState.
	else if (EventManager::KeyPressed(SDL_SCANCODE_P))
		STMA::PushState(new PauseState);

	// Background scrolling.
	for (int i = 0; i < 2; i++)
		m_gameBGArray[i].GetDstP()->x -= BGSCROLL+1;
	if (m_gameBGArray[1].GetDstP()->x <= 0)
	{
		m_gameBGArray[0].GetDstP()->x = 0;
		m_gameBGArray[1].GetDstP()->x = 1024;
	}

	for (int i = 0; i < 2; i++)
		m_gameBGBArray[i].GetDstP()->x -= BGSCROLL+5;
	if (m_gameBGBArray[1].GetDstP()->x <= 0)
	{
		m_gameBGBArray[0].GetDstP()->x = 0;
		m_gameBGBArray[1].GetDstP()->x = 1024;
	}

	// If the player is alive, update & animate.
	if(!m_isDead)
	{
		m_player->Update();
		m_player->Animate();
	}

	// If the player can shoot, fire a bullet.
	if (EventManager::KeyPressed(SDL_SCANCODE_SPACE) && m_bCanShoot && !m_isDead)
	{
		m_bCanShoot = false;
		m_vPBullets.push_back(new Bullet({ 360,80,25,22 }, { m_player->GetDstP()->x,m_player->GetDstP()->y + 35,21,18 }, 30));
		SOMA::PlaySound("pop1", 0, -1);
	}
	// Flip ability to shoot to true when Space is released.
	if (EventManager::KeyReleased(SDL_SCANCODE_SPACE))
	{
		m_bCanShoot = true;
	}
	// Enemy animation/movement.
	for (int i = 0; i < (int)m_vEnemies.size(); i++)
	{
		m_vEnemies[i]->Update();
		if (m_vEnemies[i]->GetDstP()->x < -56)
		{
			delete m_vEnemies[i];
			m_vEnemies[i] = nullptr;
			m_bENull = true;
		}
	}
	// Empty enemy vector.
	if (m_bENull) CleanVector<Enemy*>(m_vEnemies, m_bENull);
	// Update enemy spawns.
	if (m_iESpawn++ == m_iESpawnMax)
	{
		m_vEnemies.push_back(new Enemy({ 0,100,56,137 }, { WIDTH,56 + rand() % (HEIGHT - 114),56,137 }, &m_vEBullets,
			30 + rand() % 91)); // Randomizing enemy bullet spawn to every 30-120 frames.
		m_iESpawn = 0;
	}
	// Update the bullets. Player's first.
	for (int i = 0; i < (int)m_vPBullets.size(); i++)
	{
		m_vPBullets[i]->Update();
		if (m_vPBullets[i]->GetDstP()->x > WIDTH)
		{
			delete m_vPBullets[i];
			m_vPBullets[i] = nullptr;
			m_bPBNull = true;
		}
	}
	// Empty bullet vector.
	if (m_bPBNull) CleanVector<Bullet*>(m_vPBullets, m_bPBNull);
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		m_vEBullets[i]->Update();
		if (m_vEBullets[i]->GetDstP()->x < -10)
		{
			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			m_bEBNull = true;
		}
	}
	// Empty enemy bullet vector.
	if (m_bEBNull) CleanVector<Bullet*>(m_vEBullets, m_bEBNull);
	if(!m_isDead)
	{
		CheckCollision();
	}

	m_score == 15 ? m_win = true : m_win = false; // This will flip the m_win bool to true if the player reaches the indicated score
}

void GameState::CheckCollision()
{
	// Player vs. Enemy.
	SDL_Rect p = { m_player->GetDstP()->x - 60, m_player->GetDstP()->y, 60, 85 };
	SDL_Rect pT = { m_player->GetDstP()->x-100, m_player->GetDstP()->y+20, 60, 45 };
	//DEBUG//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &p);
	//DEBUG//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &pT);
	for (int i = 0; i < (int)m_vEnemies.size(); i++)
	{
		SDL_Rect e = { m_vEnemies[i]->GetDstP()->x, m_vEnemies[i]->GetDstP()->y - 43, 137, 29 };
		SDL_Rect eT = { m_vEnemies[i]->GetDstP()->x + 63, m_vEnemies[i]->GetDstP()->y - 58, 19, 60 };
		
		if (SDL_HasIntersection(&p, &e) || SDL_HasIntersection(&p, &eT) || SDL_HasIntersection(&pT, &e) || SDL_HasIntersection(&pT, &eT))
		{
			// Game over!
			cout << "Player goes boom!" << endl;
			SOMA::PlaySound("boom1", 0, -1);
			delete m_player;
			m_player = nullptr;
			m_isDead = true;				 // Flag the player as dead.
			break;
		}
	}
	
	// Player bullets vs. Enemies.
	for (int i = 0; i < (int)m_vPBullets.size(); i++)
	{
		SDL_Rect b = { m_vPBullets[i]->GetDstP()->x-50, m_vPBullets[i]->GetDstP()->y, 22, 24 };
		for (int j = 0; j < (int)m_vEnemies.size(); j++)
		{
			if (m_vEnemies[j] == nullptr) continue;
			SDL_Rect e = { m_vEnemies[j]->GetDstP()->x, m_vEnemies[j]->GetDstP()->y - 43, 137, 29 };
			SDL_Rect eT = { m_vEnemies[j]->GetDstP()->x+63, m_vEnemies[j]->GetDstP()->y-58, 19, 60 };
			//DEBUG//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &e);
			//DEBUG//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &eT);
			//DEBUG//SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &b);
			if (SDL_HasIntersection(&b, &e) || SDL_HasIntersection(&b, &eT))
			{
				SOMA::PlaySound("smack", 0, -1);
				delete m_vEnemies[j];
				m_vEnemies[j] = nullptr;
				delete m_vPBullets[i];
				m_vPBullets[i] = nullptr;
				m_bENull = true;
				m_bPBNull = true;
				m_score++;
				break;
			}
		}
	}
	
	// Empty enemy & bullet vectors.
	if (m_bENull) CleanVector<Enemy*>(m_vEnemies, m_bENull);
	if (m_bPBNull) CleanVector<Bullet*>(m_vPBullets, m_bPBNull);
	// Enemy bullets vs. player.
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		if (SDL_HasIntersection(&p, m_vEBullets[i]->GetDstP()))
		{
			// Game over!
			cout << "Player goes boom!" << endl;
			SOMA::PlaySound("boom1", 0, -1);
			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			CleanVector<Bullet*>(m_vEBullets, m_bEBNull);
			delete m_player;
			m_player = nullptr;
			m_isDead = true;
			break;
		}
	}
}


void GameState::Render()
{
	if (dynamic_cast<GameState*>(STMA::GetStates().back())) // Check to see if current state is of type GameState
	{
		State::Render();
	
		SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
		SDL_RenderClear(Engine::Instance().GetRenderer()); // Clear the screen with the draw color.

		// Render stuff. Background first.
		for (int i = 0; i < 2; i++)
			SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGText, m_gameBGArray[i].GetSrcP(), m_gameBGArray[i].GetDstP());
		for (int i = 0; i < 2; i++)
			SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGBText, m_gameBGBArray[i].GetSrcP(), m_gameBGBArray[i].GetDstP());

		
		// Player.
		if(!m_isDead)
			SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pSprText, m_player->GetSrcP(), m_player->GetDstP(), m_player->GetAngle(), &m_pivot, SDL_FLIP_NONE);
		// Player bullets.	
		for (int i = 0; i < (int)m_vPBullets.size(); i++)
		{
			SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pSprText, m_vPBullets[i]->GetSrcP(), m_vPBullets[i]->GetDstP(), 90, &m_pivot, SDL_FLIP_NONE);
		}
		// Enemies.
		for (int i = 0; i < (int)m_vEnemies.size(); i++)
		{
			SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pSprText, m_vEnemies[i]->GetSrcP(), m_vEnemies[i]->GetDstP(), -90, &m_pivot, SDL_FLIP_NONE);
		}
		// Enemy bullets.
		for (int i = 0; i < (int)m_vEBullets.size(); i++)
			SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pSprText, m_vEBullets[i]->GetSrcP(), m_vEBullets[i]->GetDstP());
		SDL_RenderPresent(Engine::Instance().GetRenderer());
	}
}

void GameState::Exit()
{
	cout << "Exiting GameState..." << endl;
	
	SDL_DestroyTexture(m_pBGText);
	SDL_DestroyTexture(m_pSprText);

	if (m_bEBNull) CleanVector<Bullet*>(m_vEBullets, m_bEBNull);
	if (m_bEBNull) CleanVector<Bullet*>(m_vPBullets, m_bPBNull);
	if (m_bENull) CleanVector<Enemy*>(m_vEnemies, m_bENull);

	delete m_player;
	m_player = nullptr;
	
	SOMA::StopMusic();

}

void GameState::Resume()
{
	cout << "Resuming GameState..." << endl;
}

void GameState::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_bRunning = false;
			break;
		}

	}

}

// End GameState
