#pragma once
#include "Particle.h"
#include <vector>

using namespace sf;

class Spawner
{
public:
	Spawner();

	void Update(float dt, RenderWindow& window, vector<Particle>& particles);

	bool IsPatternComplete() const;

	void Reset();

private:
	float m_SpawnTimer = 0.0f;
	float m_SpawnInterval = 0.2f; // bullet cooldown.
	int m_SpawnCount = 0;
	int m_maxSpawns = 114; // total particles within pattern.

	float m_bulletSpeed = -1000.0f; 
	vector<int> m_spawnX; // fixed x positions.
};

