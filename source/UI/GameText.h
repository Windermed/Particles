#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "Constants.h"

using namespace sf; // so thats why 

class Engine; // forward declaration (or else circular dependency headaches again)


// a child class of SFML's Text with changes that make it easier to work with.
class GameText : public Text
{

public:
	// our default constructor for GameTExt.
	GameText();

	// in case you just need to set the text only.
	GameText(String text);

	// if you want to set the text AND position.
	GameText(String text, Vector2f position);

	// if too, you want to do the same as above but configure the font size.
	GameText(String text, Vector2f position, unsigned int fontSize);

	// if you want to do the same as above but configure the font size.
	GameText(String text, Vector2f position, unsigned int fontSize, Color color);

	// if you want to set text but want to disregard the position.
	GameText(String text, unsigned int fontSize, Color color, bool bIsTextCentered);

	// if you too, want to do the same as above but also center the text.
	GameText(String text, Vector2f position, unsigned int fontSize, Color color, bool bIsTextCentered);

	

public:

	// just to simply things.
	void DrawText();

	// center the text.
	void CenterText(FloatRect bounds);

	void FlashText(float flashRate = 0.5f)
	{
		m_bIsTextFlashing = true;
		m_bIsTextVisible = true;
		m_flashRate = flashRate;
		m_flashTimer = 0.0f;
		m_flashDuration = -1.0f;
		m_elapsedTime = 0.0f;
	}

	// Flash for a specified duration then stop.
	void FlashText(float duration, float flashRate = 0.5f)
	{
		m_bIsTextFlashing = true;
		m_bIsTextVisible = true;
		m_flashRate = flashRate;
		m_flashTimer = 0.0f;
		m_flashDuration = duration;
		m_elapsedTime = 0.0f;
	}

	// stops flashing text.
	void StopFlash()
	{
		m_bIsTextFlashing = false;
		m_bIsTextVisible = true;
		m_flashTimer = 0.0f;
		m_elapsedTime - 0.0f;
	}

	void UpdateFlash(float dt)
	{
		if (!m_bIsTextFlashing) return;

		m_flashTimer += dt;
		m_elapsedTime += dt;

		// toggle visibility on flash rate.
		if (m_flashTimer > -m_flashRate)
		{
			m_bIsTextVisible = !m_bIsTextVisible;
			m_flashTimer = 0.0f;
		}

		// stop after specified duration if it's not infintite
		if (m_flashDuration > 0.0f && m_elapsedTime >= m_flashDuration)
		{
			StopFlash();
		}
	}

	bool IsTextFlashing() const { return m_bIsTextFlashing; }


	// Updates String and you can optionally re center it.
	//void SetText(String text, bool bRecenterText = false);

	// centers horizontally at a specific Y
	void CenterAtY(float y)
	{
		FloatRect bounds = getLocalBounds();
		setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
		setPosition(SCREEN_WIDTH / 2.0f, y);
	}

private:

	void Init(String text, Vector2f position, unsigned int fontSize, Color color);

private:

	static Font m_font; // TODO: MAKE FONT CONFIGURABLE.
	static bool m_bIsFontLoaded;

	bool m_bIsTextFlashing = false;
	bool m_bIsTextVisible = true;
	float m_flashRate = 0.5f;
	float m_flashTimer = 0.0f;

	// set to -1 for infinite.
	float m_flashDuration = -1.0f;

	float m_elapsedTime = 0.0f;
};

