#include "Engine.h"

Engine::Engine()
{
	m_Window.create(VideoMode::getDesktopMode(), "Particles", Style::Default);
}

void Engine::Run()
{
	Clock clock; // initializing clock.

	Message("Starting up Unit Tester..");
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2});
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
					int numPoints = rand() % 26 + 25;
					Vector2i clickPos(event.mouseButton.x, event.mouseButton.y);
					m_particles.push_back(Particle(m_Window, numPoints, clickPos));
				}
			}
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
		m_Window.RenderTarget::draw(p);
	}

	m_Window.display();
}