#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "GameText.h"
#include "Player.h"

using namespace sf;
using namespace std;

// helpers that i use across projects.
#define Message(input) std::cout << input << std::endl;
#define MessageNE(input) std::cout << input;
#define InputResponse(input) std::cin >> input;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

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

enum class GameState
{
	Menu = 1,
	Particles = 2,
	Other = 3
};

class Engine
{
public:
	/* Main Engine Constructor */
	Engine();

	static void Init()
	{
		if (!EngineInstance)
		{
			EngineInstance = new Engine;
		}
	}

	/* Allows us to access our engine globally. */
	static Engine* GetEngine()
	{
		return EngineInstance;
	}

	static void Shutdown()
	{
		delete EngineInstance;
		EngineInstance = nullptr;
	}

	/* Run func that will call all the private functions */
	void Run();

	/* Getter of Current Window */
	RenderWindow& GetWindow() { return m_Window; };

	/* Returns our Player object. */
	Player& GetPlayer();

	

private:

	/* Private functions for internal use only */
	void Input();
	void Update(float dtAsSeconds);
	void Draw();

public:
	/* EXTRA: Engine Flags */

	bool m_showText = false; // displays text on screen.

	bool m_showDebugText = false; // displays text on screen.

	bool m_bIsZeroGravityOn = false; // toggle for zero gravity.

private:
	
	// using this from my other project.
	static Engine* EngineInstance; // static pointer to the engine.

	GameState m_gameState = GameState::Menu;
	GameText m_menuText;

	Font m_Font;

	// PLAYER.
	Player* m_Player;

	/* A regular RenderWindow */
	RenderWindow m_Window;

	/* Vector for particles. */
	vector<Particle> m_particles;


};