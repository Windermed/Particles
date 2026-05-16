#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Engine;

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

	
	ConvexShape GetCharacter() { return m_shape; };

	void SetSpeed(float _speed) { m_Speed = _speed; };

private:
	int m_lives = 3;
	float m_hitRadius = 15.0f; // hitbox.
	ConvexShape m_shape;
	float m_Speed = 130.0f; // 120.0f is proper but for our purposes we might need to adjust it.
	float m_ShiftSpeed = 60.0f;
	Vector2f m_position;
};

