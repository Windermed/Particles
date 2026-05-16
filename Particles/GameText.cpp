#include "GameText.h"
#include "Engine.h"

GameText::GameText()
{
	// default values for our game text.
	m_font.loadFromFile("burbankbigcondensed_bold.otf");

	this->setFont(m_font);
	this->setString("Meet Potential Framework!");
	this->setCharacterSize(48);
	this->setFillColor(Color::White);
}

void GameText::CenterText(FloatRect bounds)
{
	this->setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	this->setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
}