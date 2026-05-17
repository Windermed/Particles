#include "BaseBulletSpawner.h"
#include "Engine.h"

BaseBulletSpawner::BaseBulletSpawner()
{
	// spawn positions.
	m_spawnX = { 192, 480, 760, 960, 1152, 1440, 1728 };
	m_spawnY = { 108, 270, 432, 540, 648, 810, 972 };

	//m_bIsInfinite = true;
	m_bIsProgressive = true;
	m_SpawnInterval = 0.2f;

	// maximum.
	m_minSpawnInterval = 0.5f;

	// speed rate.
	m_accelerationRate = 0.50f;
}

// update for base.
void BaseBulletSpawner::Update(float dt, RenderWindow& window, vector<Particle>& particles)
{

	m_bHasStarted = true;

	if (!ShouldSpawn(dt)) return;

	// if it is progressive, we gradually decrease the spawn interval overtime.
	if (m_bIsProgressive)
	{
		m_SpawnInterval -= m_accelerationRate * dt;
		m_SpawnInterval = std::max(m_SpawnInterval, m_minSpawnInterval);
	}

	

	// spawn bullets per interval.
	for (int i = 0; i < m_BulletCount; i++)
	{
		Vector2i spawnPos;
		float velX = 0.0f;
		float velY = 0.0f;

		// spawn direction.
		switch (m_spawnDirection)
		{
		case SpawnDirection::FromTop:
			spawnPos = Vector2i(rand() % SCREEN_WIDTH, 10); // randomized
			velX = 0.0f;
#ifdef _WIN32
			velY = -m_bulletSpeed;
#else
			velY = m_bulletSpeed;
#endif
			break;
			break;
		case SpawnDirection::FromBottom:
			spawnPos = Vector2i(rand() % SCREEN_WIDTH, SCREEN_HEIGHT - 10); // randomized.
			velX = 0.0f;
#ifdef _WIN32
			velY = m_bulletSpeed;
#endif
			break;

		case SpawnDirection::FromLeft:
			spawnPos = Vector2i(10, rand() % SCREEN_HEIGHT); // randomized.
			velX = m_bulletSpeed;
			velY = 0.0f;
			break;

		case SpawnDirection::FromRight:
			spawnPos = Vector2i(SCREEN_WIDTH - 10, m_spawnY[rand() % m_spawnY.size()]);
			velX = -m_bulletSpeed;
			velY = 0.0f;
			break;
		}

		Particle p = MakeParticle(window, spawnPos);
		p.SetVelocity(velX, velY);
		particles.push_back(p);
		m_SpawnCount++;
	}
}

bool BaseBulletSpawner::IsPatternComplete() const
{
	// it can never finish if it hasn't started.
	if (!m_bHasStarted)
		return false;

	if (m_bIsInfinite)
		return false;

	return m_bIsComplete || m_SpawnCount >= m_maxSpawns;
}

void BaseBulletSpawner::Reset()
{
	ResetBaseSettings();
}

bool BaseBulletSpawner::ShouldSpawn(float dt)
{
	m_elapsedTime += dt;
	m_SpawnTimer += dt;
	
	// Handle phases (Assuming they are enabled.)
	if (m_bEnablePhases && !m_phases.empty())
	{
		m_phaseTimer += dt;

		// check if our current phase duration has elapsed.
		if (m_currentPhase < (int)m_phases.size() && m_phaseTimer >= m_phases[m_currentPhase].duration)
		{
			m_currentPhase++;
			m_phaseTimer = 0.0f;

			// apply the next phase settings if they are avaliable.
			if (m_currentPhase < (int)m_phases.size())
			{
				m_spawnDirection = m_phases[m_currentPhase].direction;
				m_SpawnInterval = m_phases[m_currentPhase].spawnInterval;
				m_SpawnTimer = 0.0f;

				Message("Phase " << m_currentPhase << ": " << (int)m_spawnDirection)
			}
		}

		// now if all phases are done
		if (m_currentPhase >= (int)m_phases.size())
		{
			// checks if infinite mode is enabled.
			if (!m_bIsInfinite)
			{
				m_bIsComplete = true;
				return false;
			}
			// then we loop the phases assuming infinite is on.
			m_currentPhase = 0;
			m_phaseTimer = 0.0f;

		}
	}

	// if we AREN'T using phases, we check here.
	if (!m_bIsInfinite)
	{
		bool durationExpired = m_elapsedTime >= m_totalDuration;
		bool countReached = !m_bUseDurationOnly && m_SpawnCount >= m_maxSpawns;

		if (durationExpired || countReached)
		{
			m_bIsComplete = true;
			return false;
		}
	}

	if (m_SpawnTimer >= m_SpawnInterval)
	{
		m_SpawnTimer = 0.0f;
		return true;
	}

	return false;
}

Particle BaseBulletSpawner::MakeParticle(RenderWindow& window, Vector2i position)
{
	int numPoints = rand() % m_maxPoints + m_minPoints;
	Particle p(window, numPoints, position, Engine::GetEngine());

	p.SetScaling(false);
	p.ToggleGravity(false);

	// Inherit debug state from engine
	p.SetShowCollision(Engine::GetEngine()->m_bShowCollision);
	return p;
}