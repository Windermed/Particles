#include "Engine.h"
#include "Player/Player.h"
#include "Sounds/SoundManager.h"
#include "Bullet/Spawners/SpiralBulletSpawner.h"
#include "Bullet/Attacks/Bullet_AttackTest.h"
#include "Bullet/Attacks/Bullet_AttackDemo.h"


// because the compiler needs it.
Engine* Engine::EngineInstance = nullptr;

Engine::Engine()
{

	// locking it to 1080p as its annoying to work on a widescreen monitor.
	m_Window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Particles", Style::Default);
	m_Player = new Player(); // we will need to use a pointer for our player.

	// seting up UI

	// MENU
	m_menuTitle = GameText("Particles", 96, Color::White, false);
	m_menuDesc = GameText("Select the mode you want to try", 36, Color(200, 200, 200, 255), false);
	m_menuOptions = GameText("[1] Particles\n\n[2] Bullet Hell",48, Color::White, false);
	m_menuTitle.CenterAtY(SCREEN_HEIGHT / 2.0f - 200.0f);
	m_menuDesc.CenterAtY(SCREEN_HEIGHT / 2.0f - 100.0f);
	m_menuOptions.CenterAtY(SCREEN_HEIGHT / 2.0f + 50.0f);

	// WIN TEXT
	m_winText = GameText("YOU WIN!!!", 96, Color::Green, false);
	m_winScoreText = GameText("", 48, Color::Green, false);
	m_winPrompt = GameText("[R]  Play Again		[M]  Main Menu", 36, Color::White, false);
	m_winText.CenterAtY(SCREEN_HEIGHT / 2.0f - 200.0f);
	m_winPrompt.CenterAtY(SCREEN_HEIGHT / 2.0f + 160.0f);

	// GAME OVER TEXT
	m_gameOverText = GameText("GAME OVER!", 96, Color::Red, false);
	m_gameOverPrompt = GameText("[R]  Try Again 		   [M]  Main Menu", 36, Color::White, false);
	m_gameOverText.CenterAtY(SCREEN_HEIGHT / 2.0f - 80.0f);
	m_gameOverPrompt.CenterAtY(SCREEN_HEIGHT / 2.0f + 40.0f);

	// SCORE TEXT
	m_scoreText = GameText("Score: 0", 36, Color::White, false);
	m_highScoreText = GameText("Best: 0", 36, Color::White, false);
	m_scoreText.setPosition(20.0f, 20.0f);
	m_highScoreText.setPosition(20.0f, 60.0f);

	// LIVES TEXT.
	m_livesText = GameText("Lives: 3", 36, Color::White, false);
	m_livesText.setPosition(20.0f, 100.0f);


	/* BULLET HELL DEBUG HUD */
	m_attackNameText = GameText("", 36, Color(255, 255, 0, 120), false);
	m_attackNameText.CenterAtY(SCREEN_HEIGHT - 60.0f);

	// setup the icon.
	Image icon;
	if (icon.loadFromFile(ICON_PATH))
	{
		m_Window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	// setup the spawner to use default pattern use in base class
	m_activeSpawner = new BaseBulletSpawner();

	m_activeSpawner->SetInfinite(true);

	SoundManager::GetInstance();
}

void Engine::Run()
{
	Clock clock; // initializing clock.

	Message("Starting up Unit Tester..");
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 }, this);
	p.unitTests();

	// could probably do something extra here to output true or false but idk if i have time.

	Message("Unit has passed! Boot that Engine!");

	while (m_Window.isOpen())
	{
		Time elapsed = clock.restart();
		float dtAsSeconds = elapsed.asSeconds();

		this->Input();
		this->Update(dtAsSeconds);
		this->Draw();
	}

}

Player& Engine::GetPlayer()
{
	return *m_Player; // since we are using the ACTUAL player, we need to return a reference and not a copy.
}

void Engine::Input()
{
	Event event;

	// INPUTS.
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			m_Window.close();

		if (event.type == Event::KeyPressed)
		{
			m_lastKeyPressed = event.key.code;

			// closes window on escape.
			if (event.key.code == Keyboard::Escape)
				m_Window.close();

			if (m_GameState == GameState::Win || m_GameState == GameState::GameOver)
			{
				switch (m_lastKeyPressed)
				{
				case Keyboard::R:
					SetGameMode(GameMode::BulletHell);
					break;
				case Keyboard::M:
					SetGameMode(GameMode::Menu);
					break;
				}
				continue;
			}

			//  key input for each specific mode
			if (m_gameMode == GameMode::Menu)
				InputMenu();

			if (m_gameMode == GameMode::Particles)
				InputParticles(event);

			if (m_gameMode == GameMode::BulletHell)
				InputBulletHell();
		}

		// mouse input for particles.
		if (m_gameMode == GameMode::Particles && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
		{
			if (event.mouseButton.button == Mouse::Left) // mouse left click
			{
				Vector2i clickPos(event.mouseButton.x, event.mouseButton.y);
				SpawnParticleBurst(clickPos, 5);
			}
		}

		if (m_gameMode == GameMode::Particles && Mouse::isButtonPressed(Mouse::Left)) // allows for multiple particles to be spawned as you hold left.
		{
			SpawnParticle(Mouse::getPosition(m_Window));
		}
	}
}

// Engine Update.
void Engine::Update(float dtAsSeconds)
{
	// EXPERIMENTAL.
	if (m_gameMode == GameMode::Menu)
	{
		//Spawn particles from random positions
		m_menuSpawnTimer += dtAsSeconds;
		if (m_menuSpawnTimer >= m_menuSpawnInterval && m_particles.size() < 20)
		{
			m_menuSpawnTimer = 0.0f;

			// random X across the screen top.
			int x = rand() % SCREEN_WIDTH;
			Vector2i spawnPos(x, 10);

			int numPoints = rand() % 30 + 20;
			Particle p(m_Window, numPoints, spawnPos, this);
			
			// force the particles downward as there's no need for gravity.
			p.SetVelocity(0.0f, -(float)(rand() % 200 + 100));
			p.ToggleGravity(false);
			p.SetScaling(true);

			m_particles.push_back(p);
		}

		// cleanup any particles.
		CleanupVector(m_particles, dtAsSeconds);
	}
	if (m_gameMode == GameMode::BulletHell)
	{
		// if our gamestate is set to win or gameover, stop everything.
		if (m_GameState == GameState::Win || m_GameState == GameState::GameOver)
		{
			m_Player->Update(dtAsSeconds);
			return;
		}

		UpdateFlashTimer(dtAsSeconds);
		m_Player->Update(dtAsSeconds);
		m_activeSpawner->Update(dtAsSeconds, m_Window, m_particles);

		if (m_Player->GetPlayerMode() == PlayerMode::Yellow)
		{
			UpdateParticleBulletCollision(dtAsSeconds);
		}

		// bullets should always update regardless of what the player mode is set.
		m_Player->UpdateBullets(dtAsSeconds);

		UpdateParticlePlayerCollision();

		CleanupVector(m_particles, dtAsSeconds);

		if (m_activeSpawner->IsPatternComplete() && m_particles.empty() && m_GameState != GameState::Win)
		{
			m_GameState = GameState::Win;
			m_Player->StartMoveToCenter();
			m_Player->ClearBullets();
			SoundManager::GetInstance().PlaySound("snd_win_01_temp.wav", 50.0f);

			// win screen with final score.
			m_winScoreText.setString("Final Score: " + to_string(m_score));
			m_winScoreText.CenterAtY(SCREEN_HEIGHT / 2.0f + -115.0f);
		}

		// update the live HUD
		m_livesText.setString("Lives: " + to_string(m_Player->GetLives()));
	}
	else
	{
		CleanupVector(m_particles, dtAsSeconds);
	}
}


void Engine::Draw()
{
	m_Window.clear();

	switch (m_gameMode) // draw certain elements depending on GameMode
	{
	case GameMode::Menu:
		DrawMenu();
		break;
	case GameMode::Particles:
		DrawParticles();
		break;
	case GameMode::BulletHell:
		DrawBulletHell();
		DrawGameState();
		break;
	default:
		break;

	}
	
	

	m_Window.display();
}

// spawns a particle at position.
void Engine::SpawnParticle(Vector2i position)
{
	int numPoints = rand() % 26 + 25; // was gonna do 50 but rip fps
	m_particles.push_back(Particle(m_Window, numPoints, position, this));
}

// spawns a burst of particles at position
void Engine::SpawnParticleBurst(Vector2i position, int count)
{
	for (int i = 0; i < count; i++)
	{
		SpawnParticle(position);
	}
}

// resets everything for bullet hell
void Engine::ResetBulletHell()
{
	m_activeSpawner->Reset();
	m_particles.clear();
	m_Player->ResetPlayer();
	m_GameState = GameState::Playing;
	ResetScore();
	UpdateAttackNameText();
}

// handle mode transitions
void Engine::SetGameMode(GameMode gameMode)
{
	m_gameMode = gameMode;
	m_GameState = GameState::Playing;

	// clear any particles as we switch modes
	if (gameMode != GameMode::Menu)
	{
		m_particles.clear();
	}
	if (gameMode == GameMode::BulletHell)
	{
		ResetBulletHell();
	}
}

// updates the flash timer.
void Engine::UpdateFlashTimer(float dt)
{
	if (!m_bIsflashing) return;

	m_flashTimer += dt;
	if (m_flashTimer >= m_flashDuration)
	{
		m_bIsflashing = false;
		m_flashTimer = 0.0f;
	}
}

// handles yellow mode bullet collision
void Engine::UpdateParticleBulletCollision(float dt)
{
	vector<PlayerBullet>& bullets = m_Player->GetBullet();
	vector<int> bulletsToRmove;
	vector<int> particlesToRemove;

	for (int bi = 0; bi < (int)bullets.size(); bi++)
	{
		for (int pi = 0; pi < (int)m_particles.size(); pi++)
		{
			float px = m_particles[pi].GetCenter().x + 960.0f;
			float py = 540.0f - m_particles[pi].GetCenter().y;

			FloatRect particleRounds(px - m_particles[pi].GetBoundingRadius(), py - m_particles[pi].GetBoundingRadius(), m_particles[pi].GetBoundingRadius() * 2.0f, m_particles[pi].GetBoundingRadius() * 2.0f);

			// if player bullet hits particle.
			if (bullets[bi].GetBounds().intersects(particleRounds))
			{
				Message("projectile hit!");
				SoundManager::GetInstance().PlaySound("snd_bullet_hit_01.wav", 15.0f);
				AddScore(10); // let's add 10 for now.
				bulletsToRmove.push_back(bi);
				particlesToRemove.push_back(pi);
				break;
			}
		}
	}

	sort(bulletsToRmove.rbegin(), bulletsToRmove.rend());
	sort(particlesToRemove.rbegin(), particlesToRemove.rend());

	// remove any duplicates before erasing
	bulletsToRmove.erase(unique(bulletsToRmove.begin(), bulletsToRmove.end()), bulletsToRmove.end());
	particlesToRemove.erase(unique(particlesToRemove.begin(), particlesToRemove.end()), particlesToRemove.end());

	// erase bullets.
	for (int i : bulletsToRmove)
	{
		if (i < (int)bullets.size())
		{
			bullets.erase(bullets.begin() + i);
		}
	}	

	// erase particles.
	for (int i : particlesToRemove)
	{
		if (i < (int)m_particles.size())
		{
			m_particles.erase(m_particles.begin() + i);
		}
	}	
}

void Engine::UpdateParticlePlayerCollision()
{

	if (m_Player->IsGodMode()) return;

	vector<Particle>::iterator it = m_particles.begin();
	while (it != m_particles.end())
	{
		// if player gets damaged by particle.
		if (m_Player->CheckHit(it->GetCenter(), it->GetBoundingRadius())) 
		{

			// Skip if already invincible — iframes active
			if (m_Player->IsInvincible())
			{
				++it;
				continue;
			}
			
			SoundManager::GetInstance().PlaySound("snd_player_hurt_01.wav", 40.0f);
			m_Player->LoseLife(); // remove life.
			m_Player->TriggerInvincibility();
			//m_bIsflashing = true;
			//m_flashTimer = 0.0f;
			//m_Player->ResetPosition();
			it = m_particles.erase(it);

			// when player's lives are at 0.
			if (m_Player->GetLives() <= 0)
			{
				m_GameState = GameState::GameOver;
				m_activeSpawner->Reset();
				m_particles.clear();
				m_Player->ClearBullets();
				m_Player->StartMoveToCenter();
				// play a game over sound (if we find one).
				return;
			}
			continue;
		}

		++it;
	}
}

void Engine::UpdateAttackNameText()
{
	if (m_activeSpawner)
	{
		m_attackNameText.setString(string("Current Attack: ") + m_activeSpawner->GetName());

		// recenter text after we update
		m_attackNameText.CenterAtY(SCREEN_HEIGHT - 60.0f);
	}
		
}

void Engine::InputMenu()
{
	switch (m_lastKeyPressed)
	{
	case Keyboard::Num1: // sets gamemode to particles (OG Project)
		SetGameMode(GameMode::Particles);
		break;
	case Keyboard::Num2:
		SetGameMode(GameMode::BulletHell); // sets gamemode to bullet hell
		break;
	default:
		break;
	}
}

void Engine::InputParticles(Event& event)
{
	switch (m_lastKeyPressed)
	{
	case Keyboard::Space: // toggles gravity for particles.
		m_showDebugText = !m_showDebugText; // displays text.
		m_bIsZeroGravityOn = !m_bIsZeroGravityOn;

		for (Particle& p : m_particles)
		{
			p.ToggleGravity(!m_bIsZeroGravityOn);
		}
		break;

	case Keyboard::M: // returns to menu.
		SetGameMode(GameMode::Menu);
		break;
	default:
		break;
	}
}

void Engine::InputBulletHell()
{
	switch (m_lastKeyPressed)
	{
	case Keyboard::M: // return to menu.
		SetGameMode(GameMode::Menu);
		break;

	case Keyboard::Tab: // cycles between different player modes.
		m_Player->CyclePlayerMode();
		break;

	case Keyboard::G: // toggles god mode (DEVELOPER ONLY)
		m_Player->ToggleGodMode();
		break;
	case Keyboard::Num1: // temp
		delete m_activeSpawner;
		m_activeSpawner = new Bullet_AttackDemo();
		m_activeSpawner->Reset();
		m_particles.clear();
		UpdateAttackNameText();
		break;
	case Keyboard::Num2:
		delete m_activeSpawner;
		m_activeSpawner = new BaseBulletSpawner();
		m_activeSpawner->Reset();
		m_particles.clear();
		UpdateAttackNameText();
		break;
	case Keyboard::Num3:
		delete m_activeSpawner;
		m_activeSpawner = new Bullet_AttackTest();
		m_activeSpawner->Reset();
		m_particles.clear();
		UpdateAttackNameText();
		break;
	case Keyboard::F1: // PLEASE TELL ME THIS IS IT.
		ToggleCollisionDebug();
		break;
	case Keyboard::F2:
		if (auto* demo = dynamic_cast<Bullet_AttackDemo*>(m_activeSpawner))
		{
			demo->TransitionToPhase(AttackDemoPhase::Phase4);
			m_particles.clear();
		}
		break;
	default:
		break;
	}
}

void Engine::DrawParticles()
{
	for (const Particle& p : m_particles) // for every particle, draw it.
		m_Window.draw(p);

	if (m_showDebugText) // display debug text.
	{
		m_debugText.CenterText(m_debugText.getLocalBounds());
		m_Window.draw(m_debugText);
	}
}

void Engine::DrawBulletHell()
{
	for (const Particle& p : m_particles)
		m_Window.draw(p);
	
	m_Player->Draw(m_Window); // draw the player.
	m_Player->DrawBullets(m_Window); // draw the player bullets if yellow mode is on.
	m_Window.draw(m_scoreText);
	m_Window.draw(m_highScoreText);
	m_Window.draw(m_livesText);
	m_Window.draw(m_attackNameText);

	if (m_bIsflashing) // if it's flashing, flash the screen.
		m_Window.draw(m_flashOverlay);

	// Draw game state on TOP of everything.
	DrawGameState();
}

void Engine::DrawGameState()
{
	switch (m_GameState)
	{
	case GameState::Win:
		m_Window.draw(m_winText);
		m_Window.draw(m_winScoreText);
		m_Window.draw(m_winPrompt);
		break;
	case GameState::GameOver:
		m_Window.draw(m_gameOverText);
		m_Window.draw(m_gameOverPrompt);
		break;
	default:
		break;
	}
}

void Engine::ToggleCollisionDebug()
{
	m_bShowCollision = !m_bShowCollision;

	// Apply to player
	m_Player->SetShowCollision(m_bShowCollision);

	// Apply to all particles
	for (Particle& p : m_particles)
		p.SetShowCollision(m_bShowCollision);

	// Apply to bullets
	for (PlayerBullet& b : m_Player->GetBullet())
		b.SetShowCollision(m_bShowCollision);
}

