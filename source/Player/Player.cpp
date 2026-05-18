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
	m_playerSprite = GameSprite("content/textures/player/spr_heart_red.png", m_position); // default sprite

	SetPlayerMode(m_playerMode);
	m_playerSprite.setPosition(m_position);
	m_bIsPlayerInitialized = true;
}

void Player::SetPlayerMode(PlayerMode playerMode)
{


	// reset blue mode physics.
	if (m_playerMode == PlayerMode::Blue)
	{
		m_velocityY = 0.0f;
		m_bIsGrounded = false;
		m_bIsJumpHeld = false;
		m_bLandingInvincible = false;
	}

	m_playerMode = playerMode;

	switch (m_playerMode)
	{
	case PlayerMode::Red:
		m_playerSprite.Load("content/textures/player/spr_heart_red.png");
		m_playerSprite.setRotation(0.0f);

		// to avoid the sound from playing on menus or areas its not supposed to be in.
		if (m_bIsPlayerInitialized) 
		{
			SoundManager::GetInstance().PlaySound("snd_player_red_activate.wav", 30.0f);
		}
		break;

	case PlayerMode::Blue:
		m_playerSprite.Load("content/textures/player/spr_heart_blue.png");
		m_playerSprite.setRotation(0.0f);
		SoundManager::GetInstance().PlaySound("snd_falling_01.wav", 30.0f);
		break;
	case PlayerMode::Yellow:
		m_playerSprite.Load("content/textures/player/spr_heart_yellow.png");
		m_playerSprite.setRotation(180.0f);
		SoundManager::GetInstance().PlaySound("snd_player_yellow_activate.wav", 30.0f);
		break;

	default:
		m_playerSprite.Load("content/textures/spr_placeholder.png");
		m_playerSprite.setRotation(0.0f);
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

	Message("Current Player Mode: " << current);

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

void Player::StartMoveToCenter()
{
	m_bIsMovingToCenter = true;
	m_moveTarget = Vector2f(960.0f, 540.0f);
	
	// to avoid the possibility of the blue soul falling down
	// we'll reset the velocity and grounded bool.
	m_velocityY = 0.0f;
	m_bIsGrounded = false;
	m_bHasReachedCenter = false;
}

void Player::HandleInput(float dt)
{

	if (m_bInputLocked) return;

	// checks if we've reached the center. if so, player character related input is disabled.
	if (m_bHasReachedCenter) return;

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
			dir.y = -1.0f;
		}

		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		{
			m_position.x -= currentSpeed * dt;
			dir.x = -1.0f;
		}

		if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
		{
			m_position.y += currentSpeed * dt;
			dir.y = 1.0f;
		}

		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		{
			m_position.x += currentSpeed * dt; //(1.0f / 60.0f)
			dir.x = 1.0f;
		}

		/* // if we ever wanna do directional player bullets.
		if (dir.x != 0.0f || dir.y != 0.0f)
		{
			float len = sqrt(dir.x * dir.x + dir.y * dir.y);
			m_moveDirection = dir / len;
		}
		*/


		if (m_playerMode == PlayerMode::Yellow)
		{
			bool bIsZPressed = Keyboard::isKeyPressed(Keyboard::Z);

			if (bIsZPressed && !m_fireHeld)
			{
				

				Vector2f spawnPos = m_position;
				spawnPos.y += 20.0f; // offset to bottom of sprite.

				m_bullets.push_back(PlayerBullet(spawnPos, m_moveDirection));
				m_fireHeld = true;

				SoundManager::GetInstance().PlaySound("snd_laser_fire_test_01.wav", 20.0f);
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
	else if (m_playerMode == PlayerMode::Blue && !m_bIsMovingToCenter)
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
	m_lives = PLAYER_LIVES;
	m_bHasReachedCenter = false;
	m_bIsMovingToCenter = false;
	m_bLandingInvincible = false;

	ResetInputState();
	ResetPosition();
}

Vector2f Player::GetCartesianPosition() const
{
	return Vector2f(m_position.x - 960.0f, 540.0f - m_position.y);
}

void Player::Update(float dt)
{
	// if we're moving to the center via other means then skip normal input.
	if (m_bIsMovingToCenter)
	{
		// larp toward our target. slows down as it gets closer
		m_position = Vector2f(m_position.x + (m_moveTarget.x - m_position.x) * 5.0f * dt, m_position.y + (m_moveTarget.y - m_position.y) * 5.0f * dt);

		float distance = sqrt(pow(m_moveTarget.x - m_position.x, 2) +pow(m_moveTarget.y - m_position.y, 2));

		if (distance < 5.0f)
		{
			m_position = m_moveTarget;
			m_bIsMovingToCenter = false;
			m_bHasReachedCenter = true;
			m_velocityY = 0.0f;
		}

		m_playerSprite.setPosition(m_position);
		return;
	}

	HandleInput(dt);
	//m_shape.setPosition(m_position);

	// checks to see if we're on blue soul mode.
	if (m_playerMode == PlayerMode::Blue && !m_bHasReachedCenter)
	{
		// dt handles framerate
		m_velocityY += GetGravityAcceleration() * dt;

		if (m_velocityY >= m_terminalVelocity)
		{
			m_velocityY = m_terminalVelocity;
		}
		
		// while the player is falling, let's grant invincibility.
		if (m_velocityY > 0.0f && !m_bIsGrounded)
		{
			m_bLandingInvincible = true;
		}


		m_position.y += m_velocityY * dt;

		// floor collision.
		if (m_position.y >= m_floorY)
		{
			m_position.y = m_floorY;
			m_velocityY = 0.0f;
			m_bIsGrounded = true;
			m_bLandingInvincible = false;
		}
	}

	// Invincibility frames
	if (m_bIsInvincible)
	{
		m_invincibleTimer += dt;
		m_flashTimer += dt;

		// Toggle sprite visibility on flash intervals.
		if (m_flashTimer >= m_flashInterval)
		{
			m_bSpriteVisible = !m_bSpriteVisible;
			m_flashTimer = 0.0f;
		}

		// ends the invincibility.
		if (m_invincibleTimer >= m_invincibleDuration)
		{
			m_bIsInvincible = false;
			m_invincibleTimer = 0.0f;
			m_flashTimer = 0.0f;
			// sets sprite visible when finished.
			m_bSpriteVisible = true; 
			m_playerSprite.SetOpacity(255);
		}
	}

	m_playerSprite.setPosition(m_position);

	
}

void Player::Draw(RenderWindow& window)
{
	//window.draw(m_shape);
	
	// draws when visible. 
	if (m_bSpriteVisible)
		window.draw(m_playerSprite);

	// if our collision flag is enabled.
	if (m_bShowCollision) // debugging ESPECIALLY to investigate the attack demo.
	{
		CircleShape hitbox(m_hitRadius);
		hitbox.setFillColor(Color(0, 255, 0, 80));
		hitbox.setOutlineColor(Color::Green);
		hitbox.setOutlineThickness(1.0f);
		hitbox.setOrigin(m_hitRadius, m_hitRadius);
		
		Vector2f cartPos = GetCartesianPosition();
		hitbox.setPosition(cartPos.x + 960.0f, 540.0f - cartPos.y);
		window.draw(hitbox);
	}
}