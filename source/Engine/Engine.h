#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Particle/Particle.h"
#include "UI/GameText.h"
#include "Player/Player.h"
#include "Bullet/BaseBulletSpawner.h"

using namespace sf;
using namespace std;

enum class GameMode
{
	Menu = 0,
	Particles = 1,
	BulletHell = 2
};

enum class GameState
{
	Win = 0,
	GameOver = 1,
	Playing = 2,
	None = 3
};

// we should make seperate classes in the future to not clutter up Engine.
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

	// add up player score.
	void AddScore(int points)
	{
		m_score += points;
		if (m_score > m_highScore)
		{
			m_highScore = m_score;
		}

		m_scoreText.setString("Score: " + to_string(m_score));
		m_highScoreText.setString("Best: " + to_string(m_highScore));

		Message("Score: " << m_score << " High: " << m_highScore)
	}

	// reset player score.
	void ResetScore()
	{
		m_score = 0;
		m_scoreText.setString("Score: 0");
	}

	// get current player score.
	int GetScore() const { return m_score; }

	// display text hint.
	void DisplayHint(const string& text)
	{
		m_hintText.setString(text);
		m_hintText.CenterAtY(SCREEN_HEIGHT / 2.0f + 200.0f);
		m_hintText.FlashText();
	}

	// hide hint.
	void HideHint() { m_hintText.StopFlash(); }

	void StartGauntlet();
	bool IsGauntletMode() const { return m_bGauntletMode; }

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
	void UpdateAttackNameText();


	// keybinds for certain menus
	void InputMenu();
	void InputParticles(const Event::KeyPressed& event);
	void InputBulletHell();

	// draw functions of certain function.
	void DrawMenu()
	{
		//draw our particles first so that way the text can render on top of it.
		for (const Particle& p : m_particles)
			m_Window.draw(p);

		m_menuTitle.DrawText();
		m_menuDesc.DrawText();
		m_menuOptions.DrawText();
		m_menuVersion.DrawText();

		//m_Window.draw(m_menuTitle);
		//m_Window.draw(m_menuDesc);
		//m_Window.draw(m_menuOptions);
	}

	void DrawParticles();
	void DrawBulletHell();
	void DrawGameState();

	void ToggleCollisionDebug();


public:
	/* Engine Flags */

	bool m_showText = false; // displays text on screen.
	bool m_showDebugText = false; // displays text on screen.
	bool m_bIsZeroGravityOn = false; // toggle for zero gravity.
	bool m_bIsParticleMsgShowing = true;

	/* DEBUGGING */
	bool m_bShowCollision = false;

private:
	
	// using this from my other project.
	static Engine* EngineInstance; // static pointer to the engine.

	/* GameMode Engine */
	GameMode m_gameMode = GameMode::Menu;
	GameState m_GameState = GameState::Playing;
	

	/* A regular RenderWindow */
	RenderWindow m_Window;

	/* Vector for particles. */
	vector<Particle> m_particles;

	/* player object */
	Player* m_Player;

	/* Bullet Spawner */
	BaseBulletSpawner* m_activeSpawner = nullptr;

	/* HUD */
	GameText m_menuTitle;
	GameText m_menuDesc;
	GameText m_menuOptions;
	GameText m_menuVersion;

	GameText m_debugText;
	GameText m_livesText;

	// contains the text for Try Again and Main Menu
	GameText m_winLosePrompt;

	GameText m_winText;
	GameText m_gameOverText;

	GameText m_scoreText;
	GameText m_highScoreText;
	GameText m_winScoreText;

	GameText m_hintText;

	GameText m_attackNameText;

	GameText m_ParticleText;

	GameText m_GauntletMenuText;
	GameText m_GauntletOverText;

	/* TEMPORARY */
	GameText m_thankYouText;
	


	/* SCORE */
	int m_score = 0;
	int m_highScore = 0;


	/* MENU */
	float m_menuSpawnTimer = 0.0f;
	float m_menuSpawnInterval = 0.11f;


	/* Input */
	Keyboard::Key m_lastKeyPressed = Keyboard::Key::Unknown;

	/* BulletHell Flash Effect.*/
	RectangleShape m_flashOverlay;
	float m_flashTimer = 0.0f;
	float m_flashDuration = 0.3f;
	bool m_bIsflashing = false;


	/* GAUNTLET MODE */
	bool m_bGauntletMode = false;
	int m_gauntletWave = 0;

	

};