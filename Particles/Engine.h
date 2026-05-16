#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

class Engine
{
public:
	/* Main Engine Constructor */
	Engine();

	/* Run func that will call all the private functions */
	static void Run();

private:

	/* Private functions for internal use only */
	void Input();
	void Update(float dtAsSeconds);
	void Draw();


	/* A regular RenderWindow */
	RenderWindow m_Window;

	/* Vector for particles. */
	vector<Particle> m_particles;

};