#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Particle/Particle.h"
#include "UI/GameText.h"
#include "Player/Player.h"
#include "Bullet/Spawner.h"

using namespace sf;
using namespace std;

enum class GameMode
{
	Menu = 1,
	Particles = 2,
	BulletHell = 3
};

enum class GameState
{
	Win = 1,
	GameOver = 2,
	Playing = 3,
	None = 3
};

class Engine
{
public:

	/* Main Engine Constructor */
	Engine();

	/* Run func that will call all the private functions */
	void Run();

	/* Getter of Current Window */
	RenderWindow& GetWindow() { return m_Window; };

	/* Allows us to access our engine globally. */
	static Engine* GetEngine() { return EngineInstance; }

	/* Returns our Player object. */
	Player& GetPlayer();

	/* Updates the Particles */
	void UpdateParticles(float dtAsSeconds);

	/* Initializes the Engine instance. */
	static void Init()
	{
		if (!EngineInstance)
		{
			EngineInstance = new Engine;
		}
	}

	/* Deletes engine. */
	static void Shutdown()
	{
		delete EngineInstance;
		EngineInstance = nullptr;
	}
	
	template<typename T>
	void CleanupVector(vector<T>& vec, float dt)
	{
		// just so that we aren't inheriting objects that aren't from the Object class.
		static_assert(is_base_of<Object, T>::value, "T must inherit from Object");
		
		auto it = vec.begin();
		while (it != vec.end())
		{
			if (it->IsAlive())
			{
				it->Update(dt);
				++it;
			}
			else
			{
				it = vec.erase(it);
			}
		}
	}


private:

	/* Private functions for internal use only */
	void Input();
	void Update(float dtAsSeconds);
	void Draw();

	// spawn particles
	void SpawnParticle(Vector2i position);
	void SpawnParticleBurst(Vector2i position, int count = 5);

	// state management
	void ResetBulletHell();
	void SetGameMode(GameMode gameMode);

	// update
	void UpdateFlashTimer(float dt);
	void UpdateParticleBulletCollision(float dt);
	void UpdateParticlePlayerCollision();

	// keybinds for certain menus
	void InputMenu();
	void InputParticles(Event& event);
	void InputBulletHell();

	// draw functions of certain function.
	void DrawMenu() { m_Window.draw(m_menuText); }

	void DrawParticles();
	void DrawBulletHell();
	void DrawGameState();


public:
	/* EXTRA: Engine Flags */

	bool m_showText = false; // displays text on screen.
	bool m_showDebugText = false; // displays text on screen.
	bool m_bIsZeroGravityOn = false; // toggle for zero gravity.

private:
	
	// using this from my other project.
	static Engine* EngineInstance; // static pointer to the engine.

	/* GameMode Engine */
	GameMode m_gameMode = GameMode::Menu;
	GameState m_GameState = GameState::None;
	

	/* A regular RenderWindow */
	RenderWindow m_Window;

	/* Vector for particles. */
	vector<Particle> m_particles;

	/* player object */
	Player* m_Player;

	/* Bullet Spawner */
	Spawner m_Spawner;

	/* HUD */
	GameText m_menuText;
	GameText m_debugText;
	GameText m_livesText;
	GameText m_winText;
	GameText m_gameOverText;

	/* Input */
	Keyboard::Key m_lastKeyPressed = Keyboard::Unknown;

	/* BulletHell Flash Effect.*/
	RectangleShape m_flashOverlay;
	float m_flashTimer = 0.0f;
	float m_flashDuration = 0.3f;
	bool m_bIsflashing = false;

};