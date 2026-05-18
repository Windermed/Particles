#pragma once
#include "../BaseBulletSpawner.h"
#include "../Spawners/SpiralBulletSpawner.h"

enum class AttackDemoPhase
{
    Phase1,
    Phase2,
    Phase3,
    Phase4,
    Complete
};

class Bullet_AttackDemo : public BaseBulletSpawner
{
public:
    Bullet_AttackDemo();

    // base class overrides.
    void Update(float dt, RenderWindow& window, vector<Particle>& particles) override;

    bool IsPatternComplete() const override;
    void Reset() override;

    const char* GetName() const override { return "Bullet_AttackDemo"; }

public:
    void TransitionToPhase(AttackDemoPhase phase);
private:
    bool IsCurrentPhaseComplete() const;


    // one spawner set is made for each.
    BaseBulletSpawner m_SpawnerPhase1;
    BaseBulletSpawner   m_SpawnerPhase1_left;

    float m_phase1Timer = 0.0f;
    bool m_bPhase1LeftActive = false;

    float m_phase3Timer = 0.0f;
    bool m_bPhase3LeftActive = false;

    // phase 2
    BaseBulletSpawner m_SpawnerPhase2;

    // phase 3
    BaseBulletSpawner m_SpawnerPhase3;
    BaseBulletSpawner m_SpawnerPhase3_left;

    // phase 4.
    SpiralBulletSpawner m_SpawnerPhase4;

    AttackDemoPhase m_currentPhase = AttackDemoPhase::Phase1;

    bool m_bIsComplete = false;
};