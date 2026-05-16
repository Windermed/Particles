#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Engine;

enum class PlayerMode
{
	Red = 1,
	Blue = 2,
	Yellow = 3,
	MAX = 4
};

// using code from my own project as a base.
class Player
{
public:
	Player();

	void Update(float dt);
	void Draw(RenderWindow& window);
	void HandleInput(float dt);
	void ResetPosition();

	bool CheckHit(Vector2f particleCenter, float particleRadius) const;
	
	int GetLives() const { return m_lives; }
	void LoseLife() { m_lives--; }

	void ResetPlayer(); // maybe create a player state class in the future?

	Vector2f GetCartesianPosition() const;

	
	ConvexShape GetCharacter(){ return m_shape; };

	float GetFloorY() const{ return m_floorY; }

	void SetSpeed(float _speed){ m_PlayerSpeed = _speed; };

	void SetPlayerMode(PlayerMode playerMode);

	PlayerMode GetPlayerMode() const { return m_playerMode; }

	void CyclePlayerMode();

	float GetGravityAcceleration() const;

	void ToggleGodMode(){ m_bGodMode = !m_bGodMode; }

	bool IsGodMode() const { return m_bGodMode; }

	

private:

	bool m_bGodMode = true; // needed for debugging/testing.

	Texture m_playerTexture;
	Texture m_playerTexture_blue;
	Texture m_playerTexture_yellow;

	Sprite m_playerSprite;



	PlayerMode m_playerMode = PlayerMode::Blue; // red by default.

	/* BLUE MODE */
	float m_gravity = 500.0f;
	float m_velocityY = 0.0f;
	float m_terminalVelocity = 480.0f;
	float m_jumpForce = -600.0f; // tweak this if we find issues.
	bool m_bIsGrounded = false;
	float m_floorY = 1000.0f; // pixel Y position of the floor.
	bool m_jumpHeld = false;


	int m_lives = 3;
	float m_hitRadius = 15.0f; // hitbox.
	ConvexShape m_shape;
	float m_PlayerSpeed = 190.0f; // 120.0f is proper but for our purposes we might need to adjust it.
	float m_ShiftSpeed = 60.0f;
	Vector2f m_position;

	Color m_PlayerColor;
};

