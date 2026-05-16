#include "Engine.h"

// because the compiler needs it.
Engine* Engine::EngineInstance = nullptr;

Engine::Engine()
{
	// locking it to 1080p as its annoying to work on a widescreen monitor.
	m_Window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Particles", Style::Default);

	// (EXTRA) MENU

	m_Font.loadFromFile("burbankbigcondensed_bold.otf");
	
	m_menuText.setFont(m_Font);
	m_menuText.setCharacterSize(48);
	m_menuText.setFillColor(Color::White);
	m_menuText.setString("Press 1 - Particles\nPress 2 - TBD");

	// center menu text.
	m_showText = true;
	FloatRect bounds = m_menuText.getLocalBounds();
	m_menuText.CenterText(bounds);


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

void Engine::Input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		// key pressing events.
		if (event.type == Event::KeyPressed)
		{
			// closes window on escape.
			if (event.key.code == Keyboard::Escape)
			{
				m_Window.close();
			}

			if (event.type == Event::Closed)
			{
				m_Window.close();
			}

			/* TESTING FOR SMTH EXTRA */
			if (event.key.code == Keyboard::Space)
			{
				m_showDebugText = !m_showDebugText;

				m_bIsZeroGravityOn = !m_bIsZeroGravityOn;

			}

			/* (EXTRA!) A new Menu. */
			if (m_gameState == GameState::Menu)
			{
				if (event.key.code == Keyboard::Num1)
				{
					m_gameState = GameState::Particles;
				}

				if (event.key.code == Keyboard::Num2)
				{
					m_gameState = GameState::Other;
				}
			}

			// in case we want to return back to the menu
			if (event.key.code == Keyboard::M)
			{
				m_gameState = GameState::Menu;
			}
		}
		
		// regular mouse clicks. checks if also Particle GameState is present.
		if (event.type == Event::MouseButtonPressed && m_gameState == GameState::Particles)
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				for (int i = 0; i < 5; i++)
				{
					int numPoints = rand() % 26 + 25; // was gonna do 50 but rip fps.
					Vector2i clickPos(event.mouseButton.x, event.mouseButton.y);
					m_particles.push_back(Particle(m_Window, numPoints, clickPos, this));
				}
			}
		}

		// Allows you to spawn particles while holding left on ur mouse
		if (Mouse::isButtonPressed(Mouse::Left) && m_gameState == GameState::Particles)
		{
			Vector2i mousePos = Mouse::getPosition(m_Window);
			int numPoints = rand() % 26 + 25; // was gonna do 50 but rip fps

			m_particles.push_back(Particle(m_Window, numPoints, mousePos, this));
		}

	}

}

// Engine Update.
void Engine::Update(float dtAsSeconds)
{
	
	Vector2u size = m_Window.getSize();

	vector<Particle>::iterator it = m_particles.begin();
	while (it != m_particles.end())
	{
		// offscreen check.
		
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


void Engine::Draw()
{
	m_Window.clear();

	if (m_gameState == GameState::Menu)
	{
		m_Window.draw(m_menuText);
	}
	else if (m_gameState == GameState::Particles) // THE ACTUAL PROJECT.
	{
		for (const Particle& p : m_particles)
		{
			m_Window.draw(p);
		}

		if (m_showDebugText)
		{
			GameText text;

			FloatRect bounds = text.getLocalBounds();

			text.CenterText(bounds);

			m_Window.draw(text);
		}
	}
	else if (m_gameState == GameState::Other)
	{
		// TODO: DECIDE.
	}
	

	/* DISPLAY A LINE OF TEXT */

	

	m_Window.display();
}