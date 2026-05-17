#pragma once
#include "Bullet/BaseBulletSpawner.h"
#include "Bullet/Spawners/SpiralBulletSpawner.h"

// the first attack pattern (at last!!)
class Bullet_AttackTest : public BaseBulletSpawner
{
public:
	Bullet_AttackTest();

    // base class overrides.
    void Update(float dt, RenderWindow& window, vector<Particle>& particles) override;

    bool IsPatternComplete() const override;
    void Reset() override;

    const char* GetName() const override { return "CombinedAttack"; }

    BaseBulletSpawner& GetRainPattern() { return m_rain; }; // should probably move the rain pattern to another class.
    SpiralBulletSpawner& GetSpiralPattern() { return m_spiral; }

public:
    BaseBulletSpawner m_rain;
    SpiralBulletSpawner m_spiral;

};