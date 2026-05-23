#include "GameText.h"
#include "Engine.h"
#include "Core/Constants.h"

Font GameText::m_font;
bool GameText::m_bIsFontLoaded = false;

void GameText::Init(String text, Vector2f position, unsigned int fontSize, Color color)
{
	this->setString(text);
	this->setCharacterSize(fontSize);
	this->setFillColor(color);
	this->setPosition(position);
		
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
	this->setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
	this->setPosition({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
}