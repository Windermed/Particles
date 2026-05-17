#include "Bullet_AttackTest.h"

Bullet_AttackTest::Bullet_AttackTest()
{
	// for this attack, we'll test out phases.
	m_rain.SetPhaseMode(true);

	// for phase 1, let's allow bullets to fall down for 4 seconds.
	m_rain.AddPhase(SpawnDirection::FromTop, 4.0f, 0.8);

	// then after that, let's fire off bullets from the left for 4 seconds in Phase 2.
	m_rain.AddPhase(SpawnDirection::FromLeft, 4.0f, 0.6f);

	// from phase 3, we fire off bullets from the right faster for 3 seconds.
	m_rain.AddPhase(SpawnDirection::FromRight, 3.0f, 0.4f);

	// and lastly in phase 4, bullets will fall off even faster.
	m_rain.AddPhase(SpawnDirection::FromTop, 3.0f, 0.2f);

	m_rain.SetDuration(14.0f);
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