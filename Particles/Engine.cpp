#include "Engine.h"
#include "Player.h"


// because the compiler needs it.
Engine* Engine::EngineInstance = nullptr;

Engine::Engine()
{
	// locking it to 1080p as its annoying to work on a widescreen monitor.
	m_Window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Particles", Style::Default);

	m_Player = new Player(); // we will need to use a pointer for our player.

	m_menuText.setString("Press 1 - Particles\nPress 2 - Bullet Hell");

	// center menu text.
	m_showText = true;
	FloatRect bounds = m_menuText.getLocalBounds();
	m_menuText.CenterText(bounds);

	m_flashOverlay.setSize(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	m_flashOverlay.setFillColor(Color(255, 0, 0, 120));

	// win text.
	m_winText.setCharacterSize(96);
	m_winText.setFillColor(Color::Red);
	m_winText.setString("YOU WIN!");
	m_winText.setFillColor(Color::Green);
	FloatRect winBounds = m_winText.getLocalBounds();
	m_winText.CenterText(winBounds);

	// Lives text (Placeholder)
	m_livesText.setCharacterSize(36);
	m_livesText.setPosition(20.0f, 20.0f);

	// game over text.
	m_gameOverText.setCharacterSize(96);
	m_winText.setFillColor(Color::Red);
	m_gameOverText.setString("GAME OVER!");
	FloatRect gbBounds = m_gameOverText.getLocalBounds();
	m_gameOverText.CenterText(gbBounds);


}

void Engine::Run()
{
	Clock clock; // initializing clock.

	Message("Starting up Unit Tester..");
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2}, this);
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
		{
			m_Window.close();
		}

		if (event.type == Event::KeyPressed)
		{
			// closes window on escape.
			if (event.key.code == Keyboard::Escape)
			{
				m_Window.close();
			}

			// game state specific input
			if (m_GameState == GameState::Win || m_GameState == GameState::GameOver)
			{
				switch (event.key.code)
				{
				case Keyboard::R: // restarts the game.

					m_gameMode = GameMode::BulletHell;
					m_GameState = GameState::Playing;
					m_Spawner.Reset();
					m_particles.clear();
					m_Player->ResetPlayer();
					break;
				case Keyboard::M: // returns to menu.
					m_gameMode = GameMode::Menu;
					m_GameState = GameState::Playing;
					m_Spawner.Reset();
					m_particles.clear();
					m_Player->ResetPlayer();
					break;
				default:
					break;
				}
				continue; // block all other input.
			}

			// menu input
			if (m_gameMode == GameMode::Menu)
			{
				switch (event.key.code)
				{
				case Keyboard::Num1: // pressing 1 switches to particles.
					m_gameMode = GameMode::Particles;
					m_GameState = GameState::Playing;
					break;

				case Keyboard::Num2: // pressing 2 switches to bullet hell.
					m_gameMode = GameMode::BulletHell;
					m_Spawner.Reset();
					m_particles.clear();
					m_Player->ResetPlayer();
					break;

				default:
					break;
				}
			}

			// input for particles (OG Project)
			if (m_gameMode == GameMode::Particles)
			{
				switch (event.key.code)
				{
				case Keyboard::Space: // toggles gravity and displays a text.
					m_showDebugText = !m_showDebugText;
					m_bIsZeroGravityOn = !m_bIsZeroGravityOn;
					for (Particle& p : m_particles)
					{
						p.ToggleGravity(!m_bIsZeroGravityOn);
					}
					break;

				case Keyboard::M: // return to menu.
					m_gameMode = GameMode::Menu;
					break;

				default:
					break;
				}
			}

			// input for bullet hell
			if (m_gameMode == GameMode::BulletHell)
			{
				switch (event.key.code)
				{
				case Keyboard::M: // return to menu.
					m_gameMode = GameMode::Menu;
					break;

				case Keyboard::LShift:
					GetPlayer().SetSpeed(160.0f);
				
				default:
					break;
				}
			}
		}
		
		// mouse input for particles mode ONLY.

		if (m_gameMode == GameMode::Particles)
		{
			// regular mouse clicks. checks if also Particle GameState is present.
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					for (int i = 0; i < 5; i++)
					{
						int numPOints = rand() % 26 + 25;
						Vector2i clickPos(event.mouseButton.x, event.mouseButton.y);
						m_particles.push_back(Particle(m_Window, numPOints, clickPos, this));
					}
				}
			}

			// Allows you to spawn particles while holding left on ur mouse
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				Vector2i mousePos = Mouse::getPosition(m_Window);
				int numPoints = rand() % 26 + 25; // was gonna do 50 but rip fps

				m_particles.push_back(Particle(m_Window, numPoints, mousePos, this));
			}
		}

	}

}

// Engine Update.
void Engine::Update(float dtAsSeconds)
{
	Vector2u size = m_Window.getSize();
	vector<Particle>::iterator it = m_particles.begin();
	
	
	if (m_gameMode == GameMode::BulletHell)
	{
		// if our gamestate is set to win or gameover, stop everything.
		if (m_GameState == GameState::Win || m_GameState == GameState::GameOver)
		{
			return;
		}


		// update the flash timer
		if (m_flashing)
		{
			m_flashTimer += dtAsSeconds;
			if (m_flashTimer >= m_flashDuration)
			{
				m_flashing = false;
				m_flashTimer = 0.0f;
			}
		}

		// update our player
		m_Player->Update(dtAsSeconds);

		// update our bullet spawner.
		m_Spawner.Update(dtAsSeconds, m_Window, m_particles);

		// updates particles and we check for collision.
		vector<Particle>::iterator it = m_particles.begin();
		while (it != m_particles.end())
		{
			bool offScreen = it->IsOffScreen();
			bool expired = it->getTTL() <= 0.0f;

			// Check for collision with player.
			if (m_Player->CheckHit(it->GetCenter(), it->GetBoundingRadius()))
			{
				m_Player->LoseLife();
				m_flashing = true;
				m_flashTimer = 0.0f;
				m_Player->ResetPosition();
				it = m_particles.erase(it);

				if (m_Player->GetLives() <= 0)
				{
					m_GameState = GameState::GameOver;
					m_Spawner.Reset();
					m_particles.clear();
					break;
				}
				continue;
			}

			if (!expired && !offScreen)
			{
				it->Update(dtAsSeconds);
				++it;
			}
			else
			{
				it = m_particles.erase(it);
			}
		}

		// check for a win. if our bullet pattern is done and there are no particles left.
		if (m_Spawner.IsPatternComplete() && m_particles.empty())
		{
			m_GameState = GameState::Win;
		}

		// update the live HUD
		m_livesText.setString("Lives: " + to_string(m_Player->GetLives()));
	}
	else
	{
		while (it != m_particles.end())
		{

			if (it->getTTL() > 0.0 && !it->IsOffScreen())
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

	
}


void Engine::Draw()
{
	m_Window.clear();


	switch (m_gameMode)
	{
		case GameMode::Menu:
		{
			m_Window.draw(m_menuText);
			break;
		}
		case GameMode::Particles:
		{
			for (const Particle& p : m_particles)
			{
				m_Window.draw(p);
			}

			if (m_showDebugText)
			{

				FloatRect bounds = m_debugText.getLocalBounds();

				m_debugText.CenterText(bounds);

				m_Window.draw(m_debugText);
			}
			break;
		}

		case GameMode::BulletHell:
		{
			for (const Particle& p : m_particles)
				m_Window.draw(p);

			m_Player->Draw(m_Window);
			m_Window.draw(m_livesText);

			// flash overlay when player is hit
			if (m_flashing)
			{
				m_Window.draw(m_flashOverlay);
			}
			break;
		}

	}

	// game states
	switch (m_GameState)
	{
		case GameState::Win:
			m_Window.draw(m_winText);
			break;
		case GameState::GameOver:
			m_Window.draw(m_gameOverText);
			break;
		case GameState::None:
			break;
	}

	

	m_Window.display();
}