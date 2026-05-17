#include "BaseBulletSpawner.h"
#include "Engine.h"

BaseBulletSpawner::BaseBulletSpawner()
{
	m_spawnX = { 192, 480, 760, 960, 1152, 1440, 1728 };

	m_bIsInfinite = true;

	m_bIsProgressive = true;

	// we'll start off slow.
	m_SpawnInterval = 1.5f;

	// maximum.
	m_minSpawnInterval = 0.1f;

	// speed rate.
	m_accelerationRate = 0.02f;
}

// update for base.
void BaseBulletSpawner::Update(float dt, RenderWindow& window, vector<Particle>& particles)
{
	if (!ShouldSpawn(dt)) return;

	// if it is progressive, we gradually decrease the spawn interval overtime.
	if (m_bIsProgressive)
	{
		m_SpawnInterval -= m_accelerationRate * dt;
		m_SpawnInterval = std::max(m_SpawnInterval, m_minSpawnInterval);
	}

    int x = m_spawnX[rand() % m_spawnX.size()];
    Vector2i spawnPos(x, 10);

    Particle p = MakeParticle(window, spawnPos);
    p.SetVelocity(0.0f, -m_bulletSpeed);

    particles.push_back(p);
	m_SpawnCount++;
}

bool BaseBulletSpawner::IsPatternComplete() const
{
	if (m_bIsInfinite) return false; // it wont be complete since its running infinitely.

	return m_SpawnCount >= m_maxSpawns;
}

void BaseBulletSpawner::Reset()
{
	ResetBaseSettings();
}

bool BaseBulletSpawner::ShouldSpawn(float dt)
{
	m_elapsedTime += dt;
	m_SpawnTimer += dt;

	// checks if infinite mode is enabled. 
	if (!m_bIsInfinite)
	{	
		// since its not enabled, check to see if elapsed time passed total duration or not.
		if (m_elapsedTime >= m_totalDuration)
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
	return p;
}