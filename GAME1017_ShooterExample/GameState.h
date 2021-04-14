#pragma once
#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <SDL.h>

#include "Sprites.h"
#include "States.h"


class GameState : public State
{
private:
	bool m_bRunning, // Loop control flag.
		m_bENull, // These three flags check if we need to clear the respective vectors of nullptrs.
		m_bPBNull,
		m_bEBNull,
		m_isDead,
		m_win,
		m_bCanShoot; // This restricts the player from firing again unless they release the Spacebar.

	// Two backgrounds overlayed with different scrolling speeds.
	SDL_Texture* m_pBGText; 
	SDL_Texture* m_pBGBText; 
	Sprite m_gameBGArray[2];
	Sprite m_gameBGBArray[2];

	SDL_Texture* m_pSprText; // For the sprites.
	Player* m_player;

	int m_iESpawn, // The enemy spawn frame timer properties.
		m_iESpawnMax;
	int m_enemiesKilled;
	vector<Enemy*> m_vEnemies;
	vector<Bullet*> m_vPBullets;
	vector<Bullet*> m_vEBullets;
	SDL_Point m_pivot;

	int m_score;

public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
	void HandleEvents();
	void CheckCollision();
};
#endif
