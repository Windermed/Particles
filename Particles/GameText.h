#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>

using namespace sf; // so thats why 

class Engine; // forward declaration (or else circular dependency headaches again)

class GameText : public Text
{
private:

	//const String FONT_PATH;

	Font m_font; // TODO: MAKE FONT CONFIGURABLE.

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

	// center the text.
	void CenterText(FloatRect s);

};

