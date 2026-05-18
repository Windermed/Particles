#include "Bullet_AttackDemo.h"
#include "Engine.h"
#include "Sounds/SoundManager.h"

Bullet_AttackDemo::Bullet_AttackDemo()
{
    // for phase 1 the bullets here will come right up from the bottom.
    m_SpawnerPhase1.SetDirection(SpawnDirection::FromBottom);
    m_SpawnerPhase1.SetSpawnInterval(0.6f);
    m_SpawnerPhase1.SetBulletCount(3);
    m_SpawnerPhase1.SetBulletSpeed(700.0f);
    m_SpawnerPhase1.SetMaxSpawns(9999);
    m_SpawnerPhase1.SetUseDurationOnly(true);
    m_SpawnerPhase1.SetDuration(15.0f);
    m_SpawnerPhase1.SetBulletSound("snd_bullet_whoosh_01.wav", 60.0f);
    m_SpawnerPhase1.SetRareBullet("content/textures/spr_easteregg.png", 0.02f, 1.5f, 0.0f, 66);
    

    // Phase 1 left spawner. it activates after 5 seconds
    m_SpawnerPhase1_left.SetDirection(SpawnDirection::FromLeft);
    m_SpawnerPhase1_left.SetSpawnInterval(0.9f);
    m_SpawnerPhase1_left.SetBulletCount(2);
    m_SpawnerPhase1_left.SetMaxSpawns(9999);
    m_SpawnerPhase1_left.SetBulletSpeed(400.0f);
    m_SpawnerPhase1_left.SetDuration(15.0f); // lasts 10 seconds.
    m_SpawnerPhase1_left.SetUseDurationOnly(true);
    m_SpawnerPhase1_left.SetBulletSound("snd_bullet_whoosh_left_01.wav", 25.0f);

    // for Phase 2. bullets will come from the right side but on the floor.
    m_SpawnerPhase2.SetDirection(SpawnDirection::FromRight);
    m_SpawnerPhase2.SetSpawnInterval(0.7f);
    m_SpawnerPhase2.SetBulletSpeed(350.0f);
    m_SpawnerPhase2.SetDuration(4.0f);  // lasts 15 seconds.
    m_SpawnerPhase2.SetMaxSpawns(9999);
    m_SpawnerPhase2.SetUseDurationOnly(true);
    m_SpawnerPhase1.SetRareBullet("content/textures/spr_easteregg.png", 0.02f, 1.6f, 0.0f);

    // Spawn near the floor so the player is able to jump.
    m_SpawnerPhase2.SetSpawnYPositions({ 850, 900, 950, 1000 });

    // for phase 3. bullets will fall down and player must shoot them.
    m_SpawnerPhase3.SetDirection(SpawnDirection::FromTop);
    m_SpawnerPhase3.SetSpawnInterval(0.1f);
    m_SpawnerPhase3.SetBulletCount(4);
    m_SpawnerPhase3.SetMaxSpawns(9999);
    m_SpawnerPhase3.SetBulletSpeed(550.0f);
    m_SpawnerPhase3.SetDuration(30.0f); // lasts 30 seconds.
    m_SpawnerPhase3.SetProgressive(true);
    m_SpawnerPhase3.SetBulletSound("snd_bullet_falling_01.wav", 40.0f);
    m_SpawnerPhase1.SetRareBullet("content/textures/spr_easteregg.png", 0.05f, 1.5f, 0.0f, 66);
    //m_SpawnerPhase3.SetMinSpawnInterval(0.2f);
    //m_SpawnerPhase3.SetAccelerationRate(0.03f);

     // Phase 3 left spawner. it activates after 15 seconds
    m_SpawnerPhase3_left.SetDirection(SpawnDirection::FromLeft);
    m_SpawnerPhase3_left.SetSpawnInterval(0.9f);
    m_SpawnerPhase3_left.SetBulletCount(2);
    m_SpawnerPhase3_left.SetMaxSpawns(9999);
    m_SpawnerPhase3_left.SetBulletSpeed(400.0f);
    m_SpawnerPhase3_left.SetDuration(15.0f); // lasts 15 seconds.
    m_SpawnerPhase3_left.SetUseDurationOnly(true);
    m_SpawnerPhase3_left.SetBulletSound("snd_bullet_whoosh_left_01.wav", 25.0f);

    // and then on phase 4. for our final phase, a spiral will be moving across the map.
    m_SpawnerPhase4.SetSpiralSpeed(190.0f);
    m_SpawnerPhase4.SetArmsEvenly(1);
    m_SpawnerPhase4.SetSpiralRadius(250.0f);
    m_SpawnerPhase4.SetSpawnInterval(0.10f);
    m_SpawnerPhase4.SetDuration(35.0f); // lasts 35 seconds.
    m_SpawnerPhase4.SetBulletSpeed(300.0f);
    m_SpawnerPhase4.SetSpiralMoving(true, 500.0f, 0.8f);
    m_SpawnerPhase4.SetMaxSpawns(9999); // fuck it.
    //m_SpawnerPhase4.SetBulletSound("snd_bullet_spiral_01.wav");

    // Start on phase 1
    TransitionToPhase(AttackDemoPhase::Phase1);
}

void Bullet_AttackDemo::TransitionToPhase(AttackDemoPhase phase)
{
    m_currentPhase = phase;

    if (!Engine::GetEngine()) return;

    Player& player = Engine::GetEngine()->GetPlayer();
    
    switch (phase)
    {
    case AttackDemoPhase::Phase1:
        player.SetPlayerMode(PlayerMode::Red);
        Message("Phase 1 starts now!")
        break;
    case AttackDemoPhase::Phase2:
        Engine::GetEngine()->AddScore(110);
        player.SetPlayerMode(PlayerMode::Blue);
        Message("Phase 2 starts now!")
        break;
    case AttackDemoPhase::Phase3:
        Engine::GetEngine()->AddScore(160);
        player.SetPlayerMode(PlayerMode::Yellow);
        Engine::GetEngine()->DisplayHint("Press Z to shoot!");
        Message("Phase 3 starts now!")
        break;
    case AttackDemoPhase::Phase4:
        Engine::GetEngine()->AddScore(250);
        player.SetPlayerMode(PlayerMode::Red);
        Message("Phase 4 starts now!")
        //SoundManager::GetInstance().PlaySound("snd_bullet_spiral_bg_01.wav", true);
        break;
    case AttackDemoPhase::Complete:
        Engine::GetEngine()->AddScore(550);
        m_bIsComplete = true;
        Message("AttackDemo is complete!")
        //SoundManager::GetInstance().StopSound("snd_bullet_spiral_bg_01.wav");
        break;
    }
}

bool Bullet_AttackDemo::IsCurrentPhaseComplete() const
{

    switch (m_currentPhase)
    {
    case AttackDemoPhase::Phase1:
        return m_SpawnerPhase1.IsPatternComplete();
    case AttackDemoPhase::Phase2:
        return m_SpawnerPhase2.IsPatternComplete();
    case AttackDemoPhase::Phase3:
        return m_SpawnerPhase3.IsPatternComplete();
    case AttackDemoPhase::Phase4:
        return m_SpawnerPhase4.IsPatternComplete();
    default:
        return true;
    }
}

void Bullet_AttackDemo::Update(float dt, RenderWindow& window, vector<Particle>& particles)
{

    if (m_bIsComplete) return;

    // checks to see if a current phase is finished
    // if so, we move onto the next phase.
    switch (m_currentPhase)
    {
    case AttackDemoPhase::Phase1:
        m_phase1Timer += dt;

        // activates our left spawner after 5 seconds.
        if (!m_bPhase1LeftActive && m_phase1Timer >= 5.0f)
        {
            m_bPhase1LeftActive = true;
            Message("Phase 1 - Left spawner has activated!")
        }

        m_SpawnerPhase1.Update(dt, window, particles);

        if (m_bPhase1LeftActive)
        {
            m_SpawnerPhase1_left.Update(dt, window, particles); // now we update it's movements.
        }
        break;
    case AttackDemoPhase::Phase2:
        m_SpawnerPhase2.Update(dt, window, particles);
        break;
    case AttackDemoPhase::Phase3:
        m_phase3Timer += dt;
        // activates our left spawner after 5 seconds.

        if (!m_bPhase3LeftActive && m_phase3Timer >= 15.0f)
        {
            m_bPhase3LeftActive = true;
            Message("Phase 3 - Left spawner has activated!")
        }

        m_SpawnerPhase3.Update(dt, window, particles);

        if (m_bPhase3LeftActive)
        {
            m_SpawnerPhase3_left.Update(dt, window, particles); // now we update it's movements.
        }
        break;
    case AttackDemoPhase::Phase4:
        m_SpawnerPhase4.Update(dt, window, particles);
        break;
    default:
        break;
    }

    // then we check if the phase has just finished.
    if (IsCurrentPhaseComplete())
    {
        switch (m_currentPhase)
        {
        case AttackDemoPhase::Phase1:
            TransitionToPhase(AttackDemoPhase::Phase2);
            break;

        case AttackDemoPhase::Phase2:
            TransitionToPhase(AttackDemoPhase::Phase3);
            break;

        case AttackDemoPhase::Phase3:
            TransitionToPhase(AttackDemoPhase::Phase4);
            break;

        case AttackDemoPhase::Phase4:
            TransitionToPhase(AttackDemoPhase::Complete);
            break;

        default:
            break;

        }

    }
}


bool Bullet_AttackDemo::IsPatternComplete() const
{
    return m_bIsComplete;
}

// reset attack.
void Bullet_AttackDemo::Reset()
{
    m_SpawnerPhase1.Reset();
    m_SpawnerPhase1_left.Reset();
    m_SpawnerPhase2.Reset();
    m_SpawnerPhase3.Reset();
    m_SpawnerPhase3_left.Reset();
    m_SpawnerPhase4.Reset();
    m_phase1Timer = 0.0f;
    m_phase3Timer = 0.0f;
    m_bPhase1LeftActive = false;

    m_bIsComplete = false;

    TransitionToPhase(AttackDemoPhase::Phase1);
}