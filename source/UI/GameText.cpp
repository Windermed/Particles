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


// convert a hue to an Color.
Color GameText::HueToColor(float hue)
{
	float h = hue / 60.0f;
	float c = 1.0f;
	float x = c * (1.0f - fabs(fmod(h, 2.0f) - 1.0f));

	float r = 0;
	float g = 0;
	float b = 0;

	if (h < 1)
	{
		r = c;
		g = x;
	}
	else if (h < 2)
	{
		r = x;
		g = c;
	}

	else if (h < 3)
	{
		g = c;
		b = x;
	}

	else if (h < 4)
	{
		g = x;
		b = c;
	}

	else if (h < 5)
	{
		r = x;
		b = c;
	}
	else
	{
		r = c;
		b = x;
	}

	return Color(static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255));
}

void GameText::DrawText()
{
	if (!m_bIsTextVisible) return;

	if (m_bRainbowEffect)
	{
		float t = m_rainbowClock.getElapsedTime().asSeconds();
		setGlyphPreProcessor([t, this](const Text::ShapedGlyph& glyph, std::uint32_t /*style*/, Color& fillColor, Color& /*outlineColor*/, float& /*outlineThickness*/)
		{
			float hue = fmod(t * 60.0f + glyph.cluster * 25.0f, 360.0f);
			fillColor = HueToColor(hue);
		});
	}

	Engine::GetEngine()->GetWindow().draw(*this);
}

// center text on screen 
// (maybe i could make an enum that allows you to center the text anywhere? like left middle, up middle. idk.
void GameText::CenterText(FloatRect bounds)
{
	this->setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
	this->setPosition({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });
}