#pragma once
#include "Object.h"
#include "Bullet/PlayerBullet.h"
#include <vector>

using namespace sf;
using namespace std;

class Engine;

// player modes.
enum class PlayerMode
{
	Red = 1, // just regular free movement
	Blue = 2, // enables gravity, activates floor  collision.
	Yellow = 3, // gives free movement but also a shooting ability.
	MAX = 4
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


private:

	void HandleInput(float dt);
	float GetGravityAcceleration() const;

private:

	// sprites 
	Sprite m_playerSprite;
	Texture m_playerTexture;
	Texture m_playerTexture_blue;
	Texture m_playerTexture_yellow;

	// position and movement.
	PlayerMode m_playerMode = PlayerMode::Blue; // red by default.
	Vector2f m_position;
	float m_PlayerSpeed = 190.0f; // 120.0f is proper but for our purposes we might need to adjust it.
	float m_ShiftSpeed = 60.0f;

	// lives and collision
	int m_lives = 3;
	float m_hitRadius = 15.0f; // hitbox.
	bool m_bGodMode = true; // needed for debugging/testing.
	

	/* BLUE MODE PHYSICS */
	float m_velocityY = 0.0f;
	float m_terminalVelocity = 480.0f;
	float m_jumpForce = -600.0f; // tweak this if we find issues.
	float m_floorY = 1000.0f; // pixel Y position of the floor.
	bool m_bIsGrounded = false;
	bool m_bIsJumpHeld = false;



	/* YELLOW MODE SHOOTING */
	vector<PlayerBullet> m_bullets;
	Vector2f m_moveDirection = Vector2f(0.0f, -1.0f);
	bool m_fireHeld = false;

	
	/* DEPRECATED. player used to render shape. */
	ConvexShape m_shape;
	Color m_PlayerColor;
};

