#include "GameText.h"
#include "Engine.h"

GameText::GameText()
{
	// default values for our game text.
	m_font.loadFromFile("burbankbigcondensed_bold.otf");

	this->setFont(m_font);
	this->setCharacterSize(48);
	this->setFillColor(Color::White);
	this->setString("Meet Potential Framework!"); // placeholder text.
}

GameText::GameText(String text) : GameText()
{
	this->setString(text);
}

GameText::GameText(String text, Vector2f position) : GameText(text)
{
	this->setPosition(position);
}

GameText::GameText(String text, Vector2f position, unsigned int fontSize) : GameText(text, position)
{
	this->setCharacterSize(fontSize);
}

GameText::GameText(String text, Vector2f position, unsigned int fontSize, Color color) : GameText(text, position, fontSize)
{
	this->setFillColor(color);
}

void GameText::CenterText(FloatRect bounds)
{
	this->setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	this->setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
}