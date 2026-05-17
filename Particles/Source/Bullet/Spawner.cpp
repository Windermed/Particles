#include "Spawner.h"
#include "Engine.h"

Spawner::Spawner()
{
	m_spawnX = { 192, 480, 760, 960, 1152, 1440, 1728 };
}

void Spawner::Update(float dt, RenderWindow& window, vector<Particle>& particles)
{
	if (IsPatternComplete()) return;

	m_SpawnTimer += dt;

	if (m_SpawnTimer >= m_SpawnInterval)
	{
		m_SpawnTimer = 0.0f;

		// pick random x from our fixed positions.
		int x = m_spawnX[rand() % m_spawnX.size()];
		
		// spawn near the top edge
		Vector2i spawnPos(x, 10);

		Particle p(window, rand() % 20 + 30, spawnPos, Engine::GetEngine());

		p.SetVelocity(0.0f, -1200.0f);
		p.SetScaling(false); // prevents bullets (or particles) from shrinking. 
		p.ToggleGravity(false); // disable gravity for bullets.
		particles.push_back(p);
		m_SpawnCount++;
	}
}

bool Spawner::IsPatternComplete() const
{
	return m_SpawnCount >= m_maxSpawns;
}

void Spawner::Reset()
{
	m_SpawnTimer = 0.0f;
	m_SpawnCount = 0;
}