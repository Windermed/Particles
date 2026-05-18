#include "SpiralBulletSpawner.h"
#include "Engine.h"
#include "Sounds/SoundManager.h"

SpiralBulletSpawner::SpiralBulletSpawner()
{
	// allows it to spawn faster.
	m_SpawnInterval = 0.05f;
	m_bUseDurationOnly  = true;
	m_totalDuration = 10.0f;
	m_bulletSpeed = 300.0f;
	m_minPoints = 20;
	m_maxPoints = 30;
}

void SpiralBulletSpawner::Update(float dt, RenderWindow& window, vector<Particle>& particles)
{

	m_bHasStarted = true;

	// rotate angle every frame regardless of what the spawn timer is set.
	m_spiralAngle += m_spiralSpeed * dt;
	if (m_spiralAngle  >= 360.0f)
	{
		m_spiralAngle -= 360.0f;
	}

	// updates the spiral to move via center.
	if (m_bIsSpiralMoving)
	{
		m_moveTimer += dt;
		m_centerOffsetX = sin(m_moveTimer * m_moveSpeed) * m_moveRange;
	}

	if (!ShouldSpawn(dt)) return;

	// screen center in our pixel space.
	float centerX = SCREEN_WIDTH / 2.0f + m_centerOffsetX;
	float centerY = SCREEN_HEIGHT / 2.0f;

	// spawns one particle per arm
	// why did this take so long to troubleshoot...
	for (int arm = 0; arm < m_spiralArms; arm++) // FINALLY WORKS
	{
		float angleRad = (m_spiralAngle + (m_armOffset * arm)) * (3.14159f / 180.0f); 

		// Spawn position around the center
		float spawnX = centerX + cos(angleRad) * m_spiralRadius;
		float spawnY = centerY + sin(angleRad) * m_spiralRadius;

		// clamp to our screen's bounds.
		spawnX = max(0.0f, min((float)SCREEN_WIDTH, spawnX));
		spawnY = max(0.0f, min((float)SCREEN_HEIGHT, spawnY));

		Vector2i spawnPos((int)spawnX, (int)spawnY); // just in case.

		// particle spawning
		Particle p = MakeParticle(window, spawnPos);

		// velocity fires outwards from the center.
		float velX = cos(angleRad) * m_bulletSpeed;
		float velY = -(sin(angleRad) * m_bulletSpeed);

		p.SetVelocity(velX, velY);
		particles.push_back(p);
		m_SpawnCount++;

		// Play sound per particle spawn
		if (!m_bulletSound.empty())
			SoundManager::GetInstance().PlaySound(m_bulletSound);

		SoundManager::GetInstance().PlaySoundPooled("snd_bullet_spiral_02.wav", 4.0f);
	}
}

bool SpiralBulletSpawner::IsPatternComplete() const
{
	return m_bIsComplete;
}

// resets it back to 0.
void SpiralBulletSpawner::Reset()
{
	ResetBaseSettings();
	m_spiralAngle = 0.0f;
	m_moveTimer = 0.0f;
	m_centerOffsetX = 0.0f;
}