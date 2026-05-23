#include "Engine.h"
#include <fstream>
#include "Player/Player.h"
#include "Sounds/SoundManager.h"
#include "Bullet/Spawners/SpiralBulletSpawner.h"
#include "Bullet/Attacks/Bullet_AttackTest.h"
#include "Bullet/Attacks/Bullet_AttackDemo.h"


// because the compiler needs it.
Engine* Engine::EngineInstance = nullptr;

Engine::Engine()
{

	Image icon;
	float titleY = SCREEN_HEIGHT / 2.0f - 200.0f;
	float subtitleY = SCREEN_HEIGHT / 2.0f - 100.0f;
	std::string versionType = "ERROR"; // nvm prob not gonna do this so it doesnt break on linux.

	// start seeding
	srand(static_cast<unsigned int>(time(nullptr)));

	// locking it to 1080p as its annoying to work on a widescreen monitor.
	m_Window.create(VideoMode({ (unsigned int)SCREEN_WIDTH, (unsigned int)SCREEN_HEIGHT }), "Particles", Style::Default);
	m_Player = new Player(); // we will need to use a pointer for our player.

	// seting up UI


	// load splash screen text from file
	ifstream splashFile("content/splashes.txt");
	if (splashFile.is_open())
	{
		string line;
		while (getline(splashFile, line))
		{
			if (!line.empty())
			{
				m_splashes.push_back(line);
			}
		}
	}

	// MENU
	m_menuTitle = GameText("Particles", 96, Color::White, false);
	RefreshSplashText();
	m_menuDesc = GameText("Select the mode you want to try", 40, Color(200, 200, 200, 255), false);
	m_menuOptions = GameText("[1] Particles\n\n[2] Bullet Hell", 48, Color::Green, false);
	m_menuTitle.CenterAtY(SCREEN_HEIGHT / 2.0f - 200.0f);
	m_menuTitle.SetRainbowEffect(true);
	m_menuDesc.CenterAtY(SCREEN_HEIGHT / 2.0f - 65.0f);
	m_menuOptions.CenterAtY(SCREEN_HEIGHT / 2.0f + 50.0f);
	
	
	//m_menuVersion = GameText("v1.0 | dev ", 24, Color(150, 150, 150, 255), false);
	m_menuVersion = GameText("v1.1 | windermed", 24, Color(150, 150, 150, 255), false);
	m_menuVersion.CenterAtY(SCREEN_HEIGHT - 40.0f);

	// WIN TEXT
	m_winText = GameText("YOU WIN!!!", 96, Color::Green, false);
	m_winScoreText = GameText("", 48, Color::Green, false);
	m_winText.CenterAtY(SCREEN_HEIGHT / 2.0f - 200.0f);

	// TRY AGAIN MAIN MENU TEXT.
	m_winLosePrompt = GameText("[R]  Try Again 		   [M]  Main Menu", 36, Color::White, false);
	m_winLosePrompt.CenterAtY(SCREEN_HEIGHT / 2.0f + 40.0f);

	// GAME OVER TEXT
	m_gameOverText = GameText("GAME OVER!", 96, Color::Red, false);
	m_gameOverText.CenterAtY(SCREEN_HEIGHT / 2.0f - 80.0f);

	// GAUNTLET (ENDURANCE MODE) TEXT
	m_GauntletMenuText = GameText("[E] Endurance Mode", 36, Color::Yellow, false);
	m_GauntletMenuText.CenterAtY(SCREEN_HEIGHT / 2.0 + 220.0f);


	// SCORE TEXT
	m_scoreText = GameText("Score: 0", 36, Color::White, false);
	m_highScoreText = GameText("Best: 0", 36, Color::White, false);
	m_scoreText.setPosition({ 20.0f, 20.0f });
	m_highScoreText.setPosition({ 20.0f, 60.0f });

	// LIVES TEXT.
	m_livesText = GameText("HP: 3", 36, Color::White, false);
	m_livesText.setPosition({ 20.0f, 100.0f });

	// HINT TEXT.
	m_hintText = GameText("Press Z to shoot!", 48, Color::Yellow, false);
	m_hintText.CenterAtY(SCREEN_HEIGHT / 2.0f + 200.0f);

	/* BULLET HELL DEBUG HUD */
	m_attackNameText = GameText("", 36, Color(255, 255, 0, 120), false);
	m_attackNameText.CenterAtY(SCREEN_HEIGHT - 60.0f);

	// PARTICLE TEXT.
	m_ParticleText = GameText("Click anywhere to Spawn particles!\n\n Press Space to Toggle Zero Gravity.");
	m_ParticleText.CenterAtY(SCREEN_HEIGHT - 110.0f);
	if (icon.loadFromFile(ICON_PATH))
	{
		m_Window.setIcon(icon.getSize(), icon.getPixelsPtr());
	}

	
	
	// default attack when selecting bullet hell.
	m_activeSpawner = new BaseBulletSpawner();
	//m_activeSpawner->SetInfinite(true);

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
	// sf event no longer uses a default constructor in 3.1 so we will do something else instead.

	// INPUTS.
	while (const auto event = m_Window.pollEvent())
	{
		if (event->is<Event::Closed>())
			m_Window.close();

		if (const auto* keyEvent = event->getIf<Event::KeyPressed>())
		{
			m_lastKeyPressed = keyEvent->code;

			// closes window on escape.
			if (keyEvent->code == Keyboard::Key::Escape)
				m_Window.close();

			if (m_GameState == GameState::Win || m_GameState == GameState::GameOver)
			{
				switch (m_lastKeyPressed)
				{
				case Keyboard::Key::R:
					if (m_bGauntletMode)
						StartGauntlet(); // if the game ends and we want to restart in gauntlet, restart gauntlet.
					else
						SetGameMode(GameMode::BulletHell); // if not, just start up the game as normal.
					break;
				case Keyboard::Key::M:
					SetGameMode(GameMode::Menu);
					break;
				case Keyboard::Key::E:
					if (!m_bGauntletMode)
						StartGauntlet(); // if we're not already in gauntlet, start it up.
					else
						SetGameMode(GameMode::BulletHell); // if we are, then we assume we want to go back to the og attack.
				}
				continue;
			}

			//  key input for each specific mode
			if (m_gameMode == GameMode::Menu)
			{
				InputMenu();
			}
			else if (m_gameMode == GameMode::Particles)
			{
				if (const auto* kp = event->getIf<Event::KeyPressed>())
					InputParticles(*kp);
			}
			else if (m_gameMode == GameMode::BulletHell)
			{
				InputBulletHell();
			}

		}
		// mouse input for particles.
		if (m_gameMode == GameMode::Particles)
		{
			if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>())
			{
				if (mouseEvent->button == Mouse::Button::Left) // mouse left click
				{
					m_bIsParticleMsgShowing = false;
					if (m_particles.size() < 50) // particle cap
					{
						Vector2i clickPos(mouseEvent->position.x, mouseEvent->position.y);
						SpawnParticleBurst(clickPos, 5);
					}
					
				}
			}
		}
	}

	if (m_gameMode == GameMode::Particles && Mouse::isButtonPressed(Mouse::Button::Left)) // allows for multiple particles to be spawned as you hold left.
	{
		m_holdSpawnTimer += m_dt;
		if (m_holdSpawnTimer >= m_holdSpawnInterval && m_particles.size() < 150)
		{
			SpawnParticle(Mouse::getPosition(m_Window));
			m_holdSpawnTimer = 0.0f;
		}

	}
}

// Engine Update.
void Engine::Update(float dtAsSeconds)
{
	// EXPERIMENTAL.
	m_dt = dtAsSeconds;

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
			m_hintText.UpdateFlash(dtAsSeconds);
			
			if (m_hintText.IsTextFlashing() && Keyboard::isKeyPressed(Keyboard::Key::Z))
			{
				HideHint();
			}
			UpdateParticleBulletCollision(dtAsSeconds);
		}

		// bullets should always update regardless of what the player mode is set.
		m_Player->UpdateBullets(dtAsSeconds);

		UpdateParticlePlayerCollision();

		CleanupVector(m_particles, dtAsSeconds);

		if (m_activeSpawner->IsPatternComplete() && m_particles.empty() && m_GameState != GameState::Win)
		{
			// check if we are in gauntlet mode.
			if (m_bGauntletMode)
			{
				// since a wave has finished, we will start it immediately
				m_gauntletWave++;
				Message("Gauntlet Wave " << m_gauntletWave << " started!")

				delete m_activeSpawner;
				m_activeSpawner = new Bullet_AttackDemo();
				m_activeSpawner->Reset();
				m_particles.clear();

				// add bonus score to player for finishing a wave.
				AddScore(500 * m_gauntletWave);

				// update with current wave.
				m_attackNameText.setString("Gauntlet Wave " + to_string(m_gauntletWave));
				m_attackNameText.CenterAtY(SCREEN_HEIGHT - 60.0f);
			}
			else // WIN SCREEN.
			{
				m_GameState = GameState::Win;
				m_Player->StartMoveToCenter();
				m_Player->ClearBullets();
				SoundManager::GetInstance().PlaySound("snd_win_01_temp.wav", 50.0f);

				m_winText.setString("YOU WIN!!");
				//m_winText.setCharacterSize(40);
				m_winText.CenterAtY(SCREEN_HEIGHT / 2.0f - 200.0f);

				//m_thankYouText.setString("              THANK YOU    \n\nFOR AN AMAZING SEMESTER!!");
				//m_thankYouText.setCharacterSize(40);
				//m_thankYouText.CenterAtY(SCREEN_HEIGHT / 2.0f + 240.0f);
				// 
				// win screen with final score.
				m_winScoreText.setString("Final Score: " + to_string(m_score));
				m_winScoreText.CenterAtY(SCREEN_HEIGHT / 2.0f + -50.0f);

				// update positioning on game over prompt.
				m_winLosePrompt.CenterAtY(SCREEN_HEIGHT / 2.0f + 50.0f);

				// adjust the gauntlet mode text.
				m_GauntletMenuText.setString("[E] Endurance Mode");
				m_GauntletMenuText.CenterAtY(SCREEN_HEIGHT / 2.0f + 150.0f);

				//m_thankYouText.setString("              THANK YOU    \n\nFOR AN AMAZING SEMESTER!!");
				//m_thankYouText.setCharacterSize(40);
				//m_thankYouText.CenterAtY(SCREEN_HEIGHT / 2.0f + 240.0f);
			}
			
		}

		// update the live HUD
		m_livesText.setString("HP: " + to_string(m_Player->GetLives()));
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

void Engine::StartGauntlet()
{
	m_bGauntletMode = true;
	m_gauntletWave = 1;

	delete m_activeSpawner;
	m_activeSpawner = new Bullet_AttackDemo();
	m_activeSpawner->Reset();

	m_particles.clear();
	m_Player->ResetPlayer();
	m_GameState = GameState::Playing;
	
	ResetScore();
	UpdateAttackNameText();

	Message("Gauntlet has begun! We are starting at Wave " << m_gauntletWave)
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
	/* DEFAULT ATTACK */
	delete m_activeSpawner;
	m_activeSpawner = new Bullet_AttackDemo();
	m_activeSpawner->Reset();

	m_bGauntletMode = false;
	m_gauntletWave = 0;

	Message("Active spawner: " << m_activeSpawner->GetName())
	m_particles.clear();
	m_Player->ResetPlayer();
	m_GameState = GameState::Playing;
	ResetScore();
	UpdateAttackNameText();
}

// handle mode transitions
void Engine::SetGameMode(GameMode gameMode)
{
	Message("SetGameMode: " << (int)gameMode)
	m_gameMode = gameMode;
	m_GameState = GameState::Playing;

	// clear any particles as we switch modes
	if (gameMode != GameMode::Menu)
	{
		m_particles.clear();

	}
	else if (gameMode == GameMode::Menu)
	{
		RefreshSplashText(); // refresh splash text whenever we go back to the title screen.
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

			FloatRect particleRounds({ px - m_particles[pi].GetBoundingRadius(), py - m_particles[pi].GetBoundingRadius() }, { m_particles[pi].GetBoundingRadius() * 2.0f, m_particles[pi].GetBoundingRadius() * 2.0f });

			// if player bullet hits particle.
			if (bullets[bi].GetBounds().findIntersection(particleRounds))
			{
				Message("successfully hit!");

				// did we hit a rare bullet? 
				int points = m_particles[pi].IsRareBullet() ? 66 : 10; // todo: get ACTUAL rare score.
				AddScore(points);

				if (m_particles[pi].IsRareBullet())
				{
					Message("You shot a rare bullet!");
					SoundManager::GetInstance().PlaySound("snd_bullet_hit_special_01.wav", 45.0f);

				}
				else
				{
					SoundManager::GetInstance().PlaySound("snd_bullet_hit_01.wav", 15.0f);
				}
				
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

				if (m_bGauntletMode) // GAUNTLET GAME OVER SCREEN.
				{
					m_gameOverText.setString("GAUNTLET OVER!!");
					m_gameOverText.setFillColor(Color::Cyan);
					m_gameOverText.CenterAtY(SCREEN_HEIGHT / 2.0f - 200.0f);

					m_winScoreText.setString("Final Score: " + to_string(m_score));
					m_winScoreText.CenterAtY(SCREEN_HEIGHT / 2.0f + -60.0f);

					m_GauntletOverText.setString("Wave " + to_string(m_gauntletWave) + " has been reached!");
					m_GauntletOverText.CenterAtY(SCREEN_HEIGHT / 2.0f - 130.0f);

					// TRY AGAIN MAIN MENU.
					m_winLosePrompt.CenterAtY(SCREEN_HEIGHT / 2.0f + 60.0f);

					// adjust the gauntlet mode text.
					m_GauntletMenuText.setString("[E] Go Back to Regular Attack");
					m_GauntletMenuText.CenterAtY(SCREEN_HEIGHT / 2.0f + 150.0f);

					
				}
				else  // REGULAR GAME OVER SCREEN
				{
					// sets thestandard game over text back.
					m_gameOverText.setString("GAME OVER!!");
					m_gameOverText.CenterAtY(SCREEN_HEIGHT / 2.0f - 200.0f);

					m_winScoreText.setString("Recent Score: " + to_string(m_score) + " \nBest Score: " + to_string(m_highScore));
					m_winScoreText.CenterAtY(SCREEN_HEIGHT / 2.0f - 80.0f);

					// TRY AGAIN MAIN MENU.
					m_winLosePrompt.CenterAtY(SCREEN_HEIGHT / 2.0f + 60.0f);

					// adjust the gauntlet mode text.
					m_GauntletMenuText.setString("[E] Endurance Mode");
					m_GauntletMenuText.CenterAtY(SCREEN_HEIGHT / 2.0f + 150.0f);
				}
				return;
			}
			continue;
		}

		++it;
	}
}

void Engine::UpdateAttackNameText()
{
	if (!m_activeSpawner) return; // just so we don't have to double check.

	if (m_bGauntletMode)
	{
		m_attackNameText.setString("Gauntlet - Wave " + to_string(m_gauntletWave));
	}
	else
	{
		m_attackNameText.setString(string("Current Attack: ") + m_activeSpawner->GetName());
	}

	// recenter text after we update
	m_attackNameText.CenterAtY(SCREEN_HEIGHT - 60.0f);
		
}

void Engine::InputMenu()
{
	switch (m_lastKeyPressed)
	{
	case Keyboard::Key::Num1: // sets gamemode to particles (OG Project)
		SetGameMode(GameMode::Particles);
		break;
		break;
	case Keyboard::Key::Num2:
		SetGameMode(GameMode::BulletHell); // sets gamemode to bullet hell
		break;
	default:
		break;
	}
}

void Engine::InputParticles(const Event::KeyPressed& event)
{
	switch (m_lastKeyPressed)
	{
	case Keyboard::Key::Space: // toggles gravity for particles.
		m_showDebugText = !m_showDebugText; // displays text.
		m_bIsZeroGravityOn = !m_bIsZeroGravityOn;

		for (Particle& p : m_particles)
		{
			p.ToggleGravity(!m_bIsZeroGravityOn);
		}
		break;

	case Keyboard::Key::M: // returns to menu.
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
	case Keyboard::Key::M: // return to menu.
		SetGameMode(GameMode::Menu);
		break;

	case Keyboard::Key::Tab: // cycles between different player modes.
		m_Player->CyclePlayerMode();
		break;

	case Keyboard::Key::G: // toggles god mode (DEVELOPER ONLY)
		m_Player->ToggleGodMode();
		break;
	case Keyboard::Key::Num1: // temp
		delete m_activeSpawner;
		m_activeSpawner = new Bullet_AttackDemo();
		m_activeSpawner->Reset();
		m_particles.clear();
		UpdateAttackNameText();
		break;
	case Keyboard::Key::Num2:
		delete m_activeSpawner;
		m_activeSpawner = new BaseBulletSpawner();
		m_activeSpawner->Reset();
		m_particles.clear();
		UpdateAttackNameText();
		break;
	case Keyboard::Key::Num3:
		delete m_activeSpawner;
		m_activeSpawner = new Bullet_AttackTest();
		m_activeSpawner->Reset();
		m_particles.clear();
		UpdateAttackNameText();
		break;
	case Keyboard::Key::F1: // PLEASE TELL ME THIS IS IT.
		ToggleCollisionDebug();
		break;
	case Keyboard::Key::F2:
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

	if (m_bIsParticleMsgShowing)
	{
		m_ParticleText.DrawText();
	}

	
}

void Engine::DrawBulletHell()
{
	for (const Particle& p : m_particles)
		m_Window.draw(p);
	
	m_Player->Draw(m_Window); // draw the player.
	m_Player->DrawBullets(m_Window); // draw the player bullets if yellow mode is on.

	if (m_bIsflashing) // if it's flashing, flash the screen.
		m_Window.draw(m_flashOverlay);

	if (m_GameState == GameState::Playing)
	{
		if (m_hintText.IsTextFlashing())
			m_hintText.DrawText();

		m_scoreText.DrawText();
		m_highScoreText.DrawText();
		m_livesText.DrawText();
		m_attackNameText.DrawText();	
	}

	// Draw game state on TOP of everything.
	DrawGameState();
}

void Engine::DrawGameState()
{
	switch (m_GameState)
	{
	case GameState::Win:
		m_winText.DrawText();
		m_winScoreText.DrawText();
		m_winLosePrompt.DrawText();
		m_GauntletMenuText.DrawText();
		m_thankYouText.DrawText();
		break;
	case GameState::GameOver:
		m_gameOverText.DrawText();
		m_winScoreText.DrawText();
		m_winLosePrompt.DrawText();

		if (IsGauntletMode())
			m_GauntletOverText.DrawText();

		m_GauntletMenuText.DrawText();

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
	{
		p.SetShowCollision(m_bShowCollision);
	}	

	// Apply to bullets
	for (PlayerBullet& b : m_Player->GetBullet())
	{
		b.SetShowCollision(m_bShowCollision);
	}
	
}

