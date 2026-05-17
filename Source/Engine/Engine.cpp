#include "Engine.h"
#include "Player/Player.h"
#include "Sounds/SoundManager.h"
#include "Bullet/Spawners/SpiralBulletSpawner.h"
#include "Bullet/Attacks/Bullet_AttackTest.h"


// because the compiler needs it.
Engine* Engine::EngineInstance = nullptr;

Engine::Engine()
{

	// locking it to 1080p as its annoying to work on a widescreen monitor.
	m_Window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Particles", Style::Default);
	m_Player = new Player(); // we will need to use a pointer for our player.

	// UI Setup.
	m_menuText = GameText("Press 1 - Particles\nPress 2 - Bullet Hell", Vector2f(0, 0), 48, Color::White, true);
	m_winText = GameText("YOU WIN!", Vector2f(0, 0), 96, Color::Green, true);
	m_gameOverText = GameText("GAME OVER!", Vector2f(0, 0), 96, Color::Red, true);
	m_livesText = GameText("Lives: 67", Vector2f(20.0f, 20.0f), 36, Color::White, false);
	m_flashOverlay.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_flashOverlay.setFillColor(Color(255, 0, 0, 120));




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
				default:
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

	if (m_gameMode == GameMode::BulletHell)
	{
		// if our gamestate is set to win or gameover, stop everything.
		if (m_GameState == GameState::Win || m_GameState == GameState::GameOver)
		{
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

		if (m_activeSpawner->IsPatternComplete() && m_particles.empty())
		{
			m_GameState == GameState::Win;
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
		break;
	default:
		break;

	}
	
	// draw game state overlays
	DrawGameState();

	m_Window.display();
}

void Engine::UpdateParticles(float dtAsSeconds)
{
	vector<Particle>::iterator it = m_particles.begin();
	while (it != m_particles.end())
	{
		if (it->getTTL() > 0.0f && !it->IsOffScreen())
		{
			it->Update(dtAsSeconds);
			++it;
		}
		else
		{
			it = m_particles.erase(it);
		}
	}
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
}

// handle mode transitions
void Engine::SetGameMode(GameMode gameMode)
{
	m_gameMode = gameMode;

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
				SoundManager::GetInstance().PlaySound("snd_bullet_hit_01.wav");
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

	if (m_Player->GetPlayerMode() != PlayerMode::Red) return;
	if (m_Player->IsGodMode()) return;

	vector<Particle>::iterator it = m_particles.begin();
	while (it != m_particles.end())
	{
		// if player gets damaged by particle.
		if (m_Player->CheckHit(it->GetCenter(), it->GetBoundingRadius())) 
		{
			
			SoundManager::GetInstance().PlaySound("snd_player_hurt_01.wav");
			m_Player->LoseLife(); // remove life.
			m_bIsflashing = true;
			m_flashTimer = 0.0f;
			m_Player->ResetPosition();
			it = m_particles.erase(it);

			if (m_Player->GetLives() <= 0)
			{
				m_GameState = GameState::GameOver;
				m_activeSpawner->Reset();
				m_particles.clear();
				return;
			}
			continue;
		}

		++it;
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
		m_activeSpawner = new SpiralBulletSpawner();
		m_activeSpawner->Reset();
		m_particles.clear();
		break;
	case Keyboard::Num2:
		delete m_activeSpawner;
		m_activeSpawner = new BaseBulletSpawner();
		m_activeSpawner->Reset();
		m_particles.clear();
		break;
	case Keyboard::Num3:
		delete m_activeSpawner;
		m_activeSpawner = new Bullet_AttackTest();
		m_activeSpawner->Reset();
		m_particles.clear();
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
	m_Window.draw(m_livesText);

	if (m_bIsflashing) // if it's flashing, flash the screen.
		m_Window.draw(m_flashOverlay);
}

void Engine::DrawGameState()
{
	switch (m_GameState)
	{
	case GameState::Win:
		m_Window.draw(m_winText);
		break;
	case GameState::GameOver:
		m_Window.draw(m_gameOverText);
		break;
	default:
		break;
	}
}

