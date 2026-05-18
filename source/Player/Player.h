#pragma once
#include "Object.h"
#include "Bullet/PlayerBullet.h"
#include "Textures/GameSprite.h"
#include <vector>

using namespace sf;
using namespace std;

class Engine;

// player modes.
enum class PlayerMode
{
	Red = 0, // just regular free movement
	Blue = 1, // enables gravity, activates floor  collision.
	Yellow = 2, // gives free movement but also a shooting ability.
	MAX = 3
};

// using code from my own project as a base.
class Player : public Object
{
public:
	Player();

	// object overrides
	void Update(float dt) override;
	void Draw(RenderWindow& window) override;
	const char* GetName() const override { return "Player"; }

public:

	// cycle player modedw
	void CyclePlayerMode();

	// lives
	void LoseLife() { m_lives--; }
	int GetLives() const { return m_lives; }
	void ResetPlayer(); // maybe create a player state class in the future?

	// getters
	PlayerMode GetPlayerMode() const { return m_playerMode; }
	ConvexShape GetCharacter() { return m_shape; };
	float GetFloorY() const { return m_floorY; }

	// setters
	void SetSpeed(float _speed) { m_PlayerSpeed = _speed; };
	void SetPlayerMode(PlayerMode playerMode);


	// position.
	void ResetPosition();
	Vector2f GetCartesianPosition() const;

	// collision
	bool CheckHit(Vector2f particleCenter, float particleRadius) const;

	// Cheats
	void ToggleGodMode() { m_bGodMode = !m_bGodMode; }
	bool IsGodMode() const { return m_bGodMode; }
	
	
	/* YELLOW MODE */
	void UpdateBullets(float dt);
	void DrawBullets(RenderWindow& window);
	vector<PlayerBullet>& GetBullet() { return m_bullets; }
	void ClearBullets() { m_bullets.clear(); }

	/* when player gets damaged. */
	bool IsInvincible() const { return m_bIsInvincible || m_bLandingInvincible; }

	void TriggerInvincibility()
	{
		m_bIsInvincible = true;
		m_invincibleTimer = 0.0f;
		m_flashTimer = 0.0f;
		m_bSpriteVisible = true;
	}

	/* For Game over/ win */ // maybe if we had battles too lmao
	void StartMoveToCenter();
	bool IsMovingToCenter() const { return m_bIsMovingToCenter; }


	// inputs
	void ResetInputState()
	{
		m_bInputLocked = false;
		m_moveDirection = Vector2f(0.0f, -1.0f);
		m_bIsJumpHeld = false;
		m_fireHeld = false;
		m_velocityY = 0.0f;
	}

private:

	void HandleInput(float dt);
	float GetGravityAcceleration() const;

private:

	// sprites 
	GameSprite m_playerSprite;

	// position and movement.
	PlayerMode m_playerMode = PlayerMode::Red; // red by default.
	Vector2f m_position;
	float m_PlayerSpeed = 420.0f; // 120.0f is proper but for our purposes we might need to adjust it.
	float m_ShiftSpeed = 60.0f;

	// lives and collision
	int m_lives = 3;
	float m_hitRadius = 15.0f; // hitbox.
	bool m_bGodMode = true; // needed for debugging/testing.
	

	/* BLUE MODE */
	float m_velocityY = 0.0f;
	float m_terminalVelocity = 480.0f;
	float m_jumpForce = -600.0f; // tweak this if we find issues.
	float m_floorY = 1000.0f; // pixel Y position of the floor.
	bool m_bIsGrounded = false;
	bool m_bIsJumpHeld = false;
	bool m_bLandingInvincible = false;



	/* YELLOW MODE SHOOTING */
	vector<PlayerBullet> m_bullets;
	Vector2f m_moveDirection = Vector2f(0.0f, -1.0f);
	bool m_fireHeld = false;

	/* Invincibility frames */
	bool m_bIsInvincible = false;
	float m_invincibleTimer = 0.0f;

	// how many frames it'll last.
	float m_invincibleDuration = 2.0f;

	// how fast our player's sprite will flash.
	float m_flashInterval = 0.1f;
	float m_flashTimer = 0.0f;
	bool m_bSpriteVisible = true;

	// FOR GAME OVER/WIN ONLY (for now?)
	bool m_bIsMovingToCenter = false;
	bool m_bHasReachedCenter = false;
	Vector2f m_moveTarget = Vector2f(960.0f, 540.0f);
	float m_moveToCenterSpeed = 300.0f;
	
	// to address a rare bug where inputs get locked.
	bool m_bInputLocked = false;
	
	/* DEPRECATED. player used to render shape. */
	ConvexShape m_shape;
	Color m_PlayerColor;
};

