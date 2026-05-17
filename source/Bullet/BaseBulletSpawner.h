#pragma once
#include "Particle/Particle.h"
#include <vector>

using namespace sf;

class Engine;

// controls when and where bullet particles will spawn during bullethell.
class BaseBulletSpawner : public Object
{
public:
	BaseBulletSpawner();

	// object overrides
	void Update(float dt) override {};
	void Draw(RenderWindow& window) override {} // not needed.
	const char* GetName() const override { return "BaseBulletSpawner"; }

	// specific update for spawners.
	virtual void Update(float dt, RenderWindow& window, vector<Particle>& particles);

	// pattern control.
	virtual bool IsPatternComplete() const;
	virtual void Reset();

	bool ShouldSpawn(float dt);

	// creates a particle at position with current settings.
	Particle MakeParticle(RenderWindow& window, Vector2i position);


	void ResetBaseSettings()
	{
		m_SpawnTimer = 0.0f;
		m_elapsedTime = 0.0f;
		m_SpawnCount = 0;
		m_bIsComplete = false;
	}

	// setters
	void SetSpawnInterval(float interval) { m_SpawnInterval = interval; }
	void SetMaxSpawns(int max) { m_maxSpawns = max; }
	void SetBulletSpeed(float speed) { m_bulletSpeed = speed; }
	void SetDuration(float duration) { m_totalDuration = duration; }
	void SetInfinite(bool infinite) { m_bIsInfinite = infinite; }

	void SetProgressive(bool progressive) { m_bIsProgressive = progressive; }
	void SetMinSpawnInterval(float min) { m_minSpawnInterval = min; }
	void SetAccelerationRate(float rate) { m_accelerationRate = rate; }

	void SetParticleMinPoints(int min) { m_minPoints = min; }
	void SetParticleMaxPoints(int max) { m_maxPoints = max; }

	// just to make things easier.
	void SetParticlePointRange(int min, int max)
	{
		m_minPoints = min;
	}

protected:


	/** !=====================! */
	/** !== TIMING SETTINGS ==! */
	/** !=====================! */
	// this allows us to control how frequently and how long our bullets spawn.
	
	// time in seconds between bullet spawn.
	// EX: Lower values will make it spawn faster.
	float m_SpawnInterval = 0.8f;

	// The total duration of how long our bullet patterns will last before they finish.
	// NOTE: This only applies assuming they are NOT infinite.
	float m_totalDuration = 10.0f;


	/**!================================! */
	/**!== PROGRESSIVE SPEED SETTINGS ==! */
	/**!================================! */
	// enables bullets to spawn faster overtime
	// to use these, make sure that m_bIsProgressive is set to true.

	// if bullet pattern is progressive or not (speeds over time)
	bool m_bIsProgressive = false;

	// the fastest that the spawner can go when enabled.
	// it is eseentially a speed cap so the pattern doesn't become TOO fast for anyone to not beat.
	// (that is, unless you can somehow no hit it)
	float m_minSpawnInterval = 1.0f;

	// how quickly the spawn interval is able to decrease per second
	// the higher you put this, the faster bullets will spawn.
	float m_accelerationRate = 0.5f;


	/** !===================! */
	/** !== INFINITE MODE ==! */
	/** !===================! */
	// When enabled, you can essentially forget about durations, etc.
	// this will ensure the attack pattern goes on FOREVER.
	// could make for a good boss rush tho ngl.

	// Infinite mode toggle.
	bool m_bIsInfinite = false;


	/** !=====================! */
	/** !== BULLET SETTINGS ==! */
	/** !=====================! */
	// This controls the behavior of how bullets function.


	// This is the speed in which bullets will travel.
	float m_bulletSpeed = 600.0f;

	// The Maximum number of bullets that can spawn before the pattern is complete.
	// It works alongside m_totalDuration.
	int m_maxSpawns = 20;


	/** !======================! */
	/** != PARTICLE SETTINGS ==! */
	/** !======================! */
	// These settings configure how many points a particle can have.
	// do not set this too high if you value your sanity.

	// the minimum number of points that you can use to generate a particle shape.
	int m_minPoints = 20;

	// the maximum number of points that can be used to generate each particle shape.
	int m_maxPoints = 30;


	/** !!!===================!!!! */
	/** !!!== DO NOT MODIFY ==!!!! */
	/** !!!===================!!!! */
	// these are used during runtime.

	// the internal timer that tracks time since the last bullet spawn.
	float m_SpawnTimer = 0.0f;

	// this tracks how long the bullet pattern has been running.
	float m_elapsedTime = 0.0f;

	// sets to true once the pattern is finished.
	bool m_bIsComplete = false;

	// a tracker that tracks how many bullets have spawned within a bullet pattern.
	int m_SpawnCount = 0;
	
private:

	// fixed X positions that are along the edge of the screen where rain bullets
	// are able to appear. feel free to modify this if you want to change which colums the bullets fall from.
	vector<int> m_spawnX;
};

