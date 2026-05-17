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

	// if you too, want to do the same as above but also center the text.
	GameText(String text, Vector2f position, unsigned int fontSize, Color color, bool bIsTextCentered);

public:

	// center the text.
	void CenterText(FloatRect bounds);

	// Updates String and you can optionally re-center it.
	void SetText(String text, bool bRecenterText = false);

private:

	void Init(String text, Vector2f position, unsigned int fontSize, Color color);

private:

	static Font m_font; // TODO: MAKE FONT CONFIGURABLE.
	static bool m_bIsFontLoaded;
};

