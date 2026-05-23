#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Constants.h"

using namespace sf; // so thats why 

class Engine; // forward declaration (or else circular dependency headaches again)


// a child class of SFML's Text with changes that make it easier to work with.
class GameText : public Text
{

public:

	// our default constructor for GameText.
	GameText::GameText() : Text(GetFont())
	{
		// default values for our game text.
		Init("Meet Potential Framework!", Vector2f(0, 0), 36, Color::White);
	}

	// if you want to set the text AND position.
	GameText::GameText(String text) : Text(GetFont())
	{
		Init(text, Vector2f(0, 0), 36, Color::White);
	}

	// text + position (on screen) constructor. useful if you want to set the text AND position.
	GameText::GameText(String text, Vector2f position) : Text(GetFont())
	{
		Init(text, position, 36, Color::White);
	}

	// text + position (screen) + text size constructor. useful if you want to do the same as above but configure the font size.
	GameText::GameText(String text, Vector2f position, unsigned int fontSize) : Text(GetFont())
	{
		Init(text, position, fontSize, Color::White);
	}

	// text + position (screen) + text size + text color constructor. useful if you want to do the same as above but configure the font size.
	GameText::GameText(String text, Vector2f position, unsigned int fontSize, Color color) : Text(GetFont())
	{
		Init(text, position, fontSize, color);
	}

	// text + font size, text color, text is centered ahh constructor. useful if you want to set text but want to disregard the position.
	GameText::GameText(String text, unsigned int fontSize, Color color, bool bIsTextCentered) : Text(GetFont())
	{
		Init(text, Vector2f(0, 0), fontSize, color);

		if (bIsTextCentered)
			CenterText(getLocalBounds());
	}

	// text + position (screen) + text size + text color + center text constructor. useful if you want to do the same as above but also center the text.
	GameText::GameText(String text, Vector2f position, unsigned int fontSize, Color color, bool bIsTextCentered) : Text(GetFont())
	{
		Init(text, position, fontSize, color);

		if (bIsTextCentered)
		{
			CenterText(this->getLocalBounds());
		}
	}

public:

	// get font
	static Font& GetFont()
	{
		if (!m_bIsFontLoaded)
		{
			if (!m_font.openFromFile(FONT_PATH))
			{
				throw runtime_error("Font has failed to load!");
			}

			m_bIsFontLoaded = true;
		}
		return m_font;
	}

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
		setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
		setPosition({ SCREEN_WIDTH / 2.0f, y });
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

