#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Rect.hpp>

using namespace sf; // so thats why 

class Engine; // forward declaration (or else circular dependency headaches again)

class GameText : public Text
{
private:
	Font m_font;

public:

	GameText();

	void CenterText(FloatRect s);

};

