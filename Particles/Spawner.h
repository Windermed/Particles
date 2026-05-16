#pragma once
#include "Particle.h"
#include <vector>

using namespace sf;

// controls when and where bullet particles will spawn during bullethell.
class Spawner : public Object
{
public:
	Spawner();

	void Update(float dt) override {};
	void Draw(RenderWindow& window) override {} // not needed.
	const char* GetName() const override { return "BulletSpawner"; }

public:
	void Update(float dt, RenderWindow& window, vector<Particle>& particles);

	// pattern control.
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

