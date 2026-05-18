#include "GameText.h"
#include "Engine.h"
#include "Core/Constants.h"

Font GameText::m_font;
bool GameText::m_bIsFontLoaded = false;

void GameText::Init(String text, Vector2f position, unsigned int fontSize, Color color)
{
	if (!m_bIsFontLoaded)
	{
		if (!m_font.loadFromFile(FONT_PATH))
		{
			Message("ERROR: GameText has failed to load Font: " << FONT_PATH);
			throw runtime_error("Font Initialization has failed! Ensure that the font is present/is valid and try again!");
		}
		m_bIsFontLoaded = true;
	}
	this->setFont(m_font);
	this->setString(text);
	this->setCharacterSize(fontSize);
	this->setFillColor(color);
	this->setPosition(position);
		
}

// default constructor.
GameText::GameText()
{
	// default values for our game text.
	Init("Meet Potential Framework!", Vector2f(0, 0), 36, Color::White);
}

// text only constructor
GameText::GameText(String text)
{
	Init(text, Vector2f(0, 0), 36, Color::White);
}

// text + position (on screen) constructor.
GameText::GameText(String text, Vector2f position)
{
	Init(text, position, 36, Color::White);
}

// text + position (screen) + text size constructor
GameText::GameText(String text, Vector2f position, unsigned int fontSize)
{
	Init(text, position, fontSize, Color::White);
}

// text + position (screen) + text size + text color constructor.
GameText::GameText(String text, Vector2f position, unsigned int fontSize, Color color)
{
	Init(text, position, fontSize, color);
}

// text + font size, text color, text is centered ahh constructor.
GameText::GameText(String text, unsigned int fontSize, Color color, bool bIsTextCentered)
{
	Init(text, Vector2f(0, 0), fontSize, color);
	if (bIsTextCentered)
		CenterText(getLocalBounds());
}

// text + position (screen) + text size + text color + center text constructor.
GameText::GameText(String text, Vector2f position, unsigned int fontSize, Color color, bool bIsTextCentered)
{
	Init(text, position, fontSize, color);
	if (bIsTextCentered)
	{
		CenterText(this->getLocalBounds());
	}
}


void GameText::DrawText()
{
	if (!m_bIsTextVisible) return;
	Engine::GetEngine()->GetWindow().draw(*this);
}

// center text on screen 
// (maybe i could make an enum that allows you to center the text anywhere? like left middle, up middle. idk.
void GameText::CenterText(FloatRect bounds)
{
	this->setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	this->setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
}