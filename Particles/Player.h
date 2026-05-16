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

	
	ConvexShape GetCharacter() { return m_shape; };

private:
	ConvexShape m_shape;
	float m_Speed = 120.0f;
	Vector2f m_position;
};

