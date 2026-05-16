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

				// TODO: Particle spawning.
			}
		}

	}

}

void Engine::Update(float dtAsSeconds)
{
	vector<Particle>::iterator it = m_particles.begin();

}


void Engine::Draw()
{

}