#include "Engine.h"

Engine::Engine()
{
	// locking it to 1080p as its annoying to work on a widescreen monitor.
	m_Window.create(VideoMode(1920, 1080), "Particles", Style::Default);
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
			if (event.key.code == Keyboard::Escape)
			{
				m_Window.close();
			}

			/* TESTING FOR SMTH EXTRA */
			if (event.key.code == Keyboard::Space)
			{
				m_showText = !m_showText;

				m_bIsZeroGravityOn = !m_bIsZeroGravityOn;

			}
		}

		if (event.type == Event::Closed)
		{
			m_Window.close();
		}

		if (event.type == Event::MouseButtonPressed)
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
		if (Mouse::isButtonPressed(Mouse::Left))
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
	vector<Particle>::iterator it = m_particles.begin();
	while (it != m_particles.end())
	{
		if (it->getTTL() > 0.0)
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

	for (const Particle& p : m_particles)
	{
		m_Window.draw(p);
	}

	/* DISPLAY A LINE OF TEXT */

	if (m_showText)
	{
		Font font;
		font.loadFromFile("burbankbigcondensed_bold.otf");
		Text text;
		text.setFont(font);
		text.setString("Meet Potential Framework!");
		text.setCharacterSize(48);
		text.setFillColor(Color::White);
		
		FloatRect bounds = text.getLocalBounds();

		float screenW = m_Window.getSize().x;
		float screenH = m_Window.getSize().y;

		text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
		text.setPosition(screenW / 2.0f, screenH / 2.0f);

		m_Window.draw(text);
	}

	m_Window.display();
}