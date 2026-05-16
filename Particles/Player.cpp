#include "Player.h"
#include "Engine.h"

Player::Player()
{
	m_shape.setPointCount(3);
	m_shape.setPoint(0, Vector2f(0, -20));
	m_shape.setPoint(1, Vector2f(-10, 10));
	m_shape.setPoint(2, Vector2f(10, 10));
	m_shape.setFillColor(Color::Cyan);

	m_position = Vector2f(960.0f, 980.0f);
	m_shape.setPosition(m_position);
	m_shape.setOrigin(0, 0);
}

void Player::HandleInput(float dt)
{
	Keyboard EventPress;

	if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
	{
		m_position.y -= m_Speed * dt; // 240 fps monitor does not like 60.0f.
	}

	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
	{
		m_position.x -= m_Speed * dt;
	}

	if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
	{
		m_position.y += m_Speed * dt;
	}

	if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
	{
		m_position.x += m_Speed * dt; //(1.0f / 60.0f)
	}

	// clamp the position to our screen bounds.
	m_position.x = max(10.0f, min((float)SCREEN_WIDTH, m_position.x)); // updated to use the engine config instead of og project's
	m_position.y = max(10.0f, min((float)SCREEN_HEIGHT, m_position.y));
}

void Player::Update(float dt)
{
	HandleInput(dt);
	m_shape.setPosition(m_position);
}

void Player::Draw(RenderWindow& window)
{
	window.draw(m_shape);
}