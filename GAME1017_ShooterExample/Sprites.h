#pragma once
#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_mixer.h"
using namespace std;

// ------------------------------------------------------------------------------------------------
// SPRITE
class Sprite
{
private:
protected:
	SDL_Rect m_rSrc; // Rectangle for source pixels.
	SDL_Rect m_rDst; // Rectangle for destination window.
	int m_x,			// X coordinate. 
		m_y,			// Y coordinate.
		m_angle;		// Angle of sprite.
public:
	Sprite();
	Sprite(SDL_Rect s, SDL_Rect d);
	virtual ~Sprite() = default;
	SDL_Rect* GetSrcP();
	SDL_Rect* GetDstP();
	double GetAngle();
};
// ------------------------------------------------------------------------------------------------
// ANIMATED SPRITE
class AnimatedSprite : public Sprite
{
protected:
	int m_iSprite,		// Which sprite to display for animation.
		m_iSpriteMax,	// How many sprites total.
		m_iFrame,		// Frame counter.
		m_iFrameMax;	// Number of frames per sprite.

public:
	AnimatedSprite(int a, int fm, int sm, SDL_Rect s, SDL_Rect d);
	virtual ~AnimatedSprite() = default;
	virtual void Animate();
};

// ------------------------------------------------------------------------------------------------
// PLAYER
class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect s, SDL_Rect d);
	~Player() = default;
	void Update();
};

// ------------------------------------------------------------------------------------------------
// BULLET
class Bullet : public Sprite
{
private:
	int m_speed;
	bool m_active;
public:
	Bullet(SDL_Rect s, SDL_Rect d, int spd);
	void Update();
};

// ------------------------------------------------------------------------------------------------
// ENEMY
class Enemy : public AnimatedSprite
{
private:
	int m_bulletTimer,
		m_timerMax; // Basically fire rate of bullets.
	vector<Bullet*>* m_pBulletVec; // Pointer to the enemy bullet vector from Engine.
	Mix_Chunk* m_pPew;
public:
	Enemy(SDL_Rect s, SDL_Rect d, vector<Bullet*>* bVec, int fr = 120);
	void Update();
	// Add more members later.
};

// ------------------------------------------------------------------------------------------------
// BUTTON
class Button : public Sprite
{
private:
	enum BtnState { STATE_UP, STATE_OVER, STATE_DOWN } m_state; // enum defined locally.
	std::string m_keyU, m_keyO, m_keyD;
public:
	Button(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD);
	void Update();
	void Render();
	virtual void Execute();
	BtnState GetState();
};

// ------------------------------------------------------------------------------------------------
// RESUME BUTTON
class ResumeButton : public Button
{
private:
public:
	ResumeButton(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD);
	void Execute();
};

// ------------------------------------------------------------------------------------------------
// RESTART BUTTON
class RestartButton : public Button
{
private:
public:
	RestartButton(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD);
	void Execute();
};

// ------------------------------------------------------------------------------------------------
// QUIT BUTTON
class QuitButton : public Button
{
private:
public:
	QuitButton(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD);
	void Execute();
};


# endif