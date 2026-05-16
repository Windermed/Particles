#pragma once
#include "Object.h"


using namespace sf;

// a bullet that is fired by the player in yellow mode
class PlayerBullet : public Object
{
public:
	PlayerBullet(Vector2f position, Vector2f direction);

	// object overrides
	void Update(float dt) override;
	void Draw(RenderWindow& window) override;
	bool IsOffScreen() const override;
	const char* GetName() const override { return "PlayerBullet"; }

	// getters
	FloatRect GetBounds() const { return m_shape.getGlobalBounds(); }
	Vector2f GetPosition() const { return m_position; }

private:

	RectangleShape m_shape; // deprecated, uses a sprite now.
	Vector2f m_position;
	Vector2f m_velocity;
	float m_speed = 900.0f;
};

