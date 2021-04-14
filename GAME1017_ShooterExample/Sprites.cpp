#include <algorithm> // For min/max.
#include <cmath>	 // For cos/sin.
#include "Sprites.h"

#include "CollisionManager.h"
#include "EndState.h"
#include "Engine.h"
#include "EventManager.h"
#include "GameState.h"
#include "SoundManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "TitleState.h"
using namespace std;

Sprite::Sprite() {}
Sprite::Sprite(SDL_Rect s, SDL_Rect d) :m_rSrc(s), m_rDst(d) {}
SDL_Rect* Sprite::GetSrcP() { return &m_rSrc; }
SDL_Rect* Sprite::GetDstP() { return &m_rDst; }
double Sprite::GetAngle() { return m_angle; }

AnimatedSprite::AnimatedSprite(int a, int fm, int sm, SDL_Rect s, SDL_Rect d) :
	Sprite(s, d), m_iFrameMax(fm), m_iSpriteMax(sm)
{
	// Added this here (and not in header line) just because. Note: initializers ARE resolved before body statements.
	m_angle = a;
	m_iSprite = m_iFrame = 0; // Chaining assignments to the same value.
}

void AnimatedSprite::Animate()
{
	m_iFrame++;
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
		}
	}
	m_rSrc.x = m_rSrc.w * m_iSprite;
}
// ------------------------------------------------------------------------------------------------
// PLAYER
Player::Player(SDL_Rect s, SDL_Rect d) : AnimatedSprite(90, 8, 4, s, d) {}

void Player::Update()
{
	// Player animation/movement.
	//Animate();
	if (EventManager::KeyHeld(SDL_SCANCODE_A) && GetDstP()->x > GetDstP()->h)
		GetDstP()->x -= PSPEED;
	else if (EventManager::KeyHeld(SDL_SCANCODE_D) && GetDstP()->x < WIDTH / 2)
		GetDstP()->x += PSPEED;
	if (EventManager::KeyHeld(SDL_SCANCODE_W) && GetDstP()->y > 0)
		GetDstP()->y -= PSPEED;
	else if (EventManager::KeyHeld(SDL_SCANCODE_S) && GetDstP()->y < HEIGHT - GetDstP()->w)
		GetDstP()->y += PSPEED;
}

// ------------------------------------------------------------------------------------------------
// BULLET
Bullet::Bullet(SDL_Rect s, SDL_Rect d, int spd) :
	Sprite(s, d), m_speed(spd), m_active(true) {}

void Bullet::Update()
{
	m_rDst.x += m_speed;
}


// ------------------------------------------------------------------------------------------------
// ENEMY
Enemy::Enemy(SDL_Rect s, SDL_Rect d, vector<Bullet*>* bVec, int fr) : AnimatedSprite(-90, 8, 3, s, d),
m_pBulletVec(bVec), m_bulletTimer(0), m_timerMax(fr) {}

void Enemy::Update()
{
	Animate();
	m_rDst.x -= 4;
	if (m_bulletTimer++ == m_timerMax)
	{
		m_bulletTimer = 0;
		m_pBulletVec->push_back(new Bullet({ 360,50,28,26 }, { m_rDst.x,m_rDst.y - 35,18,16 }, -10));
		SOMA::PlaySound("pop2", 0, -1);
	}
}

// ------------------------------------------------------------------------------------------------
// BUTTON

Button::Button(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD) : Sprite(s, d),
m_keyU(keyU), m_keyO(keyO), m_keyD(keyD), m_state(STATE_UP) {}

void Button::Update()
{
	bool col = COMA::PointAABBCheck(EVMA::GetMousePos(), m_rDst);
	EVMA::SetCursor(col ? SDL_SYSTEM_CURSOR_HAND : SDL_SYSTEM_CURSOR_ARROW);
	switch (m_state)
	{
	case STATE_UP:
		if (col) m_state = STATE_OVER;
		break;
	case STATE_OVER:
		if (!col) m_state = STATE_UP;
		else if (col && EVMA::MousePressed(1)) m_state = STATE_DOWN; // 1 is left mouse.
		break;
	case STATE_DOWN:
		if (EVMA::MouseReleased(1)) // Left mouse released.
		{
			SoundManager::PlaySound("ding", 0, -1);
			if (col)
			{
				m_state = STATE_OVER;
				Execute();
				EVMA::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
			}
			else
				m_state = STATE_UP;
		}
		break;
	}

}

void Button::Execute()
{
	STMA::ChangeState(new GameState());
}

void Button::Render()
{
	switch (m_state)
	{
	case STATE_UP:
		SDL_RenderCopy(Engine::Instance().GetRenderer(), TextureManager::GetTexture(m_keyU), &m_rSrc, &m_rDst);
		break;
	case STATE_OVER:
		SDL_RenderCopy(Engine::Instance().GetRenderer(), TextureManager::GetTexture(m_keyO), &m_rSrc, &m_rDst);
		break;
	case STATE_DOWN:
		SDL_RenderCopy(Engine::Instance().GetRenderer(), TextureManager::GetTexture(m_keyD), &m_rSrc, &m_rDst);
		break;
	}
}

Button::BtnState Button::GetState()
{
	return m_state;
}

// ------------------------------------------------------------------------------------------------
// RESUME BUTTON

ResumeButton::ResumeButton(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD) : Button(s, d, keyU, keyO, keyD){}

void ResumeButton::Execute()
{
	STMA::PopState();
}
// ------------------------------------------------------------------------------------------------
// RESTART BUTTON

RestartButton::RestartButton(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD) : Button(s, d, keyU, keyO, keyD) {}

void RestartButton::Execute()
{
	STMA::ChangeState(new TitleState());
}

// ------------------------------------------------------------------------------------------------
// QUIT BUTTON

QuitButton::QuitButton(SDL_Rect s, SDL_Rect d, const char* keyU, const char* keyO, const char* keyD) : Button(s, d, keyU, keyO, keyD) {}

void QuitButton::Execute()
{
	Engine::Instance().SetRunning(false);
}
