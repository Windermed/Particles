#include "Player.h"
#include "Engine.h"
#include "Sounds/SoundManager.h"

Player::Player()
{

	/*
		m_shape.setPointCount(3);

		m_PlayerColor = Color::Red;

		m_shape.setPoint(0, Vector2f(0, -20));
		m_shape.setPoint(1, Vector2f(-10, 10));
		m_shape.setPoint(2, Vector2f(10, 10));
		m_shape.setFillColor(m_PlayerColor);

		m_position = Vector2f(960.0f, 980.0f);
		m_shape.setPosition(m_position);
		m_shape.setOrigin(0, 0);
	*/

	m_playerTexture.loadFromFile("content/textures/player/spr_heart_red.png");
	m_playerTexture_blue.loadFromFile("content/textures/player/spr_heart_blue.png");
	m_playerTexture_yellow.loadFromFile("content/textures/player/spr_heart_yellow.png");

	this->SetPlayerMode(m_playerMode);

	//m_playerSprite.setScale(0.05f, 0.05f); // scale the sprite down.
	m_playerSprite.setPosition(m_position);
}

void Player::SetPlayerMode(PlayerMode playerMode)
{
	// a guard just in case.
	if (m_playerMode == PlayerMode::MAX)
	{
		return;
	}

	// reset blue mode physics.
	if (m_playerMode == PlayerMode::Blue)
	{
		m_velocityY = 0.0f;
		m_bIsGrounded = false;
		m_bIsJumpHeld = false;
	}
	m_playerMode = playerMode;

	switch (m_playerMode)
	{
	case PlayerMode::Red:
		m_playerSprite.setTexture(m_playerTexture);
		m_playerSprite.setRotation(0.0f);
		break;

	case PlayerMode::Blue:
		m_playerSprite.setTexture(m_playerTexture_blue);
		m_playerSprite.setRotation(0.0f);
		break;
	case PlayerMode::Yellow:
		m_playerSprite.setTexture(m_playerTexture_yellow);
		m_playerSprite.setRotation(180.0f);
		break;

	default:
		break;
	}

	// recenter our origin after texture swap just in case.
	FloatRect bounds = m_playerSprite.getLocalBounds();
	m_playerSprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

void Player::CyclePlayerMode()
{
	// we cast to an int, increment, wrap around, then cast back.
	int current = static_cast<int>(m_playerMode);
	int next = (current + 1) % static_cast<int>(PlayerMode::MAX);

	if (next >= static_cast<int>(PlayerMode::MAX))
	{
		next = 0;
	}

	SetPlayerMode(static_cast<PlayerMode>(next));
}

float Player::GetGravityAcceleration() const
{
	if (m_velocityY <= -240.0f) 
	{
		return 720.0f;
	}
	else if (m_velocityY <= -60.0f)
	{
		return 1800.0f;
	}

	else if (m_velocityY <= 30.0f)
	{
		return 720.0f;
	}

	else if (m_velocityY < 480.0f)
	{
		return 2160.0f;
	}

	else
	{
		return 0.0f; // terminal velocity.
	}
}

void Player::UpdateBullets(float dt)
{
	vector<PlayerBullet>::iterator it = m_bullets.begin();
	while (it != m_bullets.end())
	{
		if (it->IsOffScreen())
		{
			it = m_bullets.erase(it);
		}
		else
		{
			it->Update(dt);
			++it;
		}
	}
}

void Player::DrawBullets(RenderWindow& window)
{
	for (PlayerBullet& b : m_bullets)
	{
		b.Draw(window);
	}
}

void Player::HandleInput(float dt)
{
	Keyboard EventPress;

	bool bShiftHeld = Keyboard::isKeyPressed(Keyboard::LShift) || Keyboard::isKeyPressed(Keyboard::RShift);
	bool bJumpKeyHeld = Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space);

	float currentSpeed = bShiftHeld ? m_ShiftSpeed : m_PlayerSpeed;

	// ensures that the player cant hold jump if blue to fly.
	if (!bJumpKeyHeld)
	{
		m_bIsJumpHeld = false;
	}



	if (m_playerMode == PlayerMode::Red || m_playerMode == PlayerMode::Yellow)
	{
		// Track movement direction for bullet firing
		Vector2f dir(0.0f, 0.0f);

		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
		{
			m_position.y -= currentSpeed * dt; // 240 fps monitor does not like 60.0f.
		}

		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			m_position.x -= currentSpeed * dt;
		}

		if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
		{
			m_position.y += currentSpeed * dt;
		}

		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			m_position.x += currentSpeed * dt; //(1.0f / 60.0f)
		}


		if (m_playerMode == PlayerMode::Yellow)
		{
			bool bIsZPressed = Keyboard::isKeyPressed(Keyboard::Z);

			if (bIsZPressed && !m_fireHeld)
			{
				

				Vector2f spawnPos = m_position;
				spawnPos.y += 20.0f; // offset to bottom of sprite.

				m_bullets.push_back(PlayerBullet(spawnPos, m_moveDirection));
				m_fireHeld = true;

				SoundManager::GetInstance().PlaySound("snd_laser_fire_test_01.wav");
			}
			
			if (!bIsZPressed)
			{
				m_fireHeld = false;
			}
		}

		// clamp the position to our screen bounds.
		m_position.x = max(10.0f, min((float)SCREEN_WIDTH, m_position.x)); // updated to use the engine config instead of og project's
		m_position.y = max(10.0f, min((float)SCREEN_HEIGHT, m_position.y));

		
	}
	else if (m_playerMode == PlayerMode::Blue)
	{
		// horizontal movement
		float currentSpeed = bShiftHeld ? m_ShiftSpeed : m_PlayerSpeed;

		// jump only when we are grounded.
		if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (bJumpKeyHeld && !m_bIsJumpHeld && m_bIsGrounded)
			{
				m_velocityY = m_jumpForce;
				m_bIsGrounded = false;
				m_bIsJumpHeld = true;
			}
			
		}

		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			m_position.x -= currentSpeed * dt;
		}

		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			m_position.x += currentSpeed * dt;
		}

		m_position.x = max(10.0f, min((float)SCREEN_WIDTH, m_position.x));
	}
	
}

void Player::ResetPosition()
{
	m_position = Vector2f(960.0f, 980.0f);
	//m_shape.setPosition(m_position);

	m_playerSprite.setPosition(m_position);
}

bool Player::CheckHit(Vector2f particleCenter, float particleRadius) const
{
	Vector2f playerPos = GetCartesianPosition();
	float dx = particleCenter.x - playerPos.x;
	float dy = particleCenter.y - playerPos.y;

	float dist = sqrt(dx * dx + dy * dy);

	// hit if player is within the combined radius of the particle
	return dist < (m_hitRadius + particleRadius);
}

void Player::ResetPlayer()
{
	m_lives = 3;
	ResetPosition();
}

Vector2f Player::GetCartesianPosition() const
{
	return Vector2f(m_position.x - 960.0f, 540.0f - m_position.y);
}

void Player::Update(float dt)
{
	HandleInput(dt);
	//m_shape.setPosition(m_position);

	// checks to see if we're on blue soul mode.
	if (m_playerMode == PlayerMode::Blue)
	{
		// dt handles framerate
		m_velocityY += GetGravityAcceleration() * dt;

		if (m_velocityY >= m_terminalVelocity)
		{
			m_velocityY = m_terminalVelocity;
		}

		m_position.y += m_velocityY * dt;

		// floor collision.
		if (m_position.y >= m_floorY)
		{
			m_position.y = m_floorY;
			m_velocityY = 0.0f;
			m_bIsGrounded = true;
		}
	}

	m_playerSprite.setPosition(m_position);

	// change color depending on player (soul) mode.
	//m_shape.setFillColor(m_playerMode == PlayerMode::Red ? Color::Red : Color::Blue);

	
}

void Player::Draw(RenderWindow& window)
{
	//window.draw(m_shape);
	window.draw(m_playerSprite);
}