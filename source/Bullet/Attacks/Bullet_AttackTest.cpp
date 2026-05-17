#include "Bullet_AttackTest.h"

Bullet_AttackTest::Bullet_AttackTest()
{
	// nerf the rain a bit
	m_rain.SetSpawnInterval(1.2f);
	m_rain.SetMaxSpawns(15);

	// 
	m_spiral.SetSpiralSpeed(90.0f);
	m_spiral.SetSpiralArms(2);
	m_spiral.SetDuration(12.0f);
}

void Bullet_AttackTest::Update(float dt, RenderWindow& window, vector<Particle>& particles)
{
	// let both patterns run at the same time.
	m_rain.Update(dt, window, particles);
	m_spiral.Update(dt, window, particles);
}

bool Bullet_AttackTest::IsPatternComplete() const
{
	return m_rain.IsPatternComplete() && m_spiral.IsPatternComplete();
}

void Bullet_AttackTest::Reset()
{
	m_rain.Reset();
	m_spiral.Reset();
}