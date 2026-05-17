#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
using namespace sf;

// this is the base class for all of our game objects.
class Object
{
public:
	virtual ~Object() {};

	// core loop
	virtual void Update(float dt) = 0;
	virtual void Draw(RenderWindow& window) = 0;

	virtual bool IsOffScreen() const { return false; }
	virtual float GetTTL() const { return 1.0f; }
	virtual bool IsAlive() const { return GetTTL() > 0.0f && !IsOffScreen(); }

	virtual const char* GetName() const { return "Object"; }

	void SetShowCollision(bool show) { m_bShowCollision = show; }
	bool IsShowingCollision() const { return m_bShowCollision; }

protected:
	bool m_bShowCollision = false;
};

