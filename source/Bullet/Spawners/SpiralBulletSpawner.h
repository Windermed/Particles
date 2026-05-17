#pragma once
#include "Bullet/BaseBulletSpawner.h"

class SpiralBulletSpawner : public BaseBulletSpawner
{
public:
	SpiralBulletSpawner();

	// overrides from base spawner
	void Update(float dt, RenderWindow& window, vector<Particle>& particles) override;

	bool IsPatternComplete() const override;
	
	void Reset() override;

	const char* GetName() const override { return "SpiralBulletSpawner"; }

	// sets the speed of how the spiral rotates.
	void SetSpiralSpeed(float speed) { m_spiralSpeed = speed; }

	// The distance to spawn point.
	void SetSpiralRadius(float radius) { m_spiralRadius = radius; }

	// the number of arms that are fired at the same time. feel free to increase/decrease if necessary.
	void SetSpiralArms(int arms) { m_spiralArms = arms; }

	// How far apart each spiral arm is from each other.
	void SetArmOffset(float offset) { m_armOffset = offset; }

	// if you want both arms to be the same.
	void SetArmsEvenly(int arms)
	{
		m_spiralArms = arms;
		m_armOffset = 360.0f / arms;
	}

protected:

	/** !=====================! */
	/** !== SPIRAL SETTINGS ==! */
	/** !=====================! */
	// this controls the shape and spiral pattern behavior.

	// the speed in which our spiral rotates.
	float m_spiralSpeed = 120.0f;

	// The Distance to spawn point.
	float m_spiralRadius = 300.0f;

	// number of arms that get fired at the same time.
	int m_spiralArms = 3;

	// how far the arms are apart of each other.
	float m_armOffset = 120.0f;

	// !=== DO NOT MODIFY ===!
	// this gets updated every frame
	float m_spiralAngle = 0.0f;
};