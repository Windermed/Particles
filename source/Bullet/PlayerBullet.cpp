#include "PlayerBullet.h"
#include <cmath>
PlayerBullet::PlayerBullet(Vector2f position, Vector2f direction)
{
	m_position = position;

	// normalize direction
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length != 0)
	{
		direction /= length;
	}

	m_velocity = direction * m_speed;

	// small laser shape 

	m_shape.setSize(Vector2f(4.0f, 16.0f));
	m_shape.setFillColor(Color::Yellow);
	m_shape.setOutlineColor(Color::White);
	m_shape.setOutlineThickness(1.0f);

	// center origin
	m_shape.setOrigin(2.0f, 8.0f);

	// rotate to match direction.
	float angle = atan2(direction.y, direction.x) * 180.0f / 3.14159f;
	m_shape.setRotation(angle + 90.0f);

	m_shape.setPosition(m_position);
}

void PlayerBullet::Update(float dt)
{
	m_position += m_velocity * dt;
	m_shape.setPosition(m_position);
}

void PlayerBullet::Draw(RenderWindow& window)
{
	window.draw(m_shape);
}

bool PlayerBullet::IsOffScreen() const
{
	return m_position.x < 0 || m_position.x > SCREEN_WIDTH || m_position.y < 0 || m_position.y > SCREEN_HEIGHT;
}