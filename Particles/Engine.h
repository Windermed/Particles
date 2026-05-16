#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

// helpers that i use across projects.
#define Message(input) std::cout << input << std::endl;
#define MessageNE(input) std::cout << input;
#define InputResponse(input) std::cin >> input;

enum class TextColor
{
	Default,
	Black, 
	Red, 
	Green, 
	Yellow, 
	Blue, 
	Magenta, 
	Cyan, 
	White,
	BrightBlack,
	BrightRed,
	BrightGreen,
	BrightYellow,
	BrightBlue,
	BrightMagenta,
	BrightCyan,
	BrightWhite
};

class Engine
{
public:
	/* Main Engine Constructor */
	Engine();

	/* Run func that will call all the private functions */
	void Run();

private:

	

	/* Private functions for internal use only */
	void Input();
	void Update(float dtAsSeconds);
	void Draw();


	/* A regular RenderWindow */
	RenderWindow m_Window;

	/* Vector for particles. */
	vector<Particle> m_particles;

	bool m_showText = false; // EXPERIMENTAL.

};