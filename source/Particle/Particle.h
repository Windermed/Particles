#pragma once
#include "Object.h"
#include "Matrices.h"
#include "Textures/GameSprite.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

const float G = 1000;      //Gravity
const float TTL = 5.0; //Time To Live
const float SCALE = 0.999;

using namespace Matrices;
using namespace sf;

class Engine;

// after project is submitted, we should turn this into a seperate class.
class Particle : public Drawable, public Object
{
public:
	Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, Engine* owningEngine);

    //SFML override.
    virtual void draw(RenderTarget& target, RenderStates states) const override;

    // our object interface declarations
    void Update(float dt) override;
    void Draw(RenderWindow& window) override {};
    bool IsOffScreen() const override;
    float GetTTL() const override { return m_ttl; }
    const char* GetName() const override { return "Particle"; }

public:
    Vector2f GetCenter() const { return m_centerCoordinate; }
    float GetBoundingRadius() const;
    // Returns velocity, not Y position
    float GetParticleY() { return m_vy; };

    float GetVelocityY() const { return m_vy; }

    // setters
    void SetVelocity(float vx, float vy) { m_vx = vx; m_vy = vy; };
    void SetScaling(bool bEnabled) { m_shouldScale = bEnabled; }
    void ToggleGravity(bool bEnabled) { m_UseGravity = bEnabled; }


    /* Functions for unit testing */
    bool almostEqual(double a, double b, double eps = 0.0001);
    void unitTests();


    /* EXTRA */
    void SetSprite(const string& spritePath, float scale = 1.0f, float rotation = 0.0f)
    {
        bool bLoaded = m_sprite.Load(spritePath);
        m_bUseSprite = bLoaded;

        if (bLoaded)
        {
            m_sprite.SetScale(scale);
            m_sprite.setRotation(rotation);
        }
    }

    void SetRareBullet(bool bIsRare) { m_bIsRareBullet = bIsRare; }
    bool IsRareBullet() const { return m_bIsRareBullet; }

    bool IsUsingSprite() const { return m_bUseSprite; }

private:

    /* rotate Particle by theta radians counter - clockwise */
    /* construct a RotationMatrix R, left mulitply it to m_A */
    void rotate(double theta);

    /** Scale the size of the Particle by factor c */
    /** construct a ScalingMatrix S, left multiply it to m_A */
    void scale(double c);

    /** shift the Particle by (xShift, yShift) coordinates */
    /** construct a TranslationMatrix T, add it to m_A */
    void translate(double xShift, double yShift);

private:

    // engine reference. (maybe remove as its not needed anymore)
    Engine* m_engine; // this will allow me to point back to the current engine instance.

    // flags
    bool m_shouldScale = true; // whether or not a particle is allowed to scale.
    bool m_UseGravity = true; // whether or not a particle can use gravity.

    // state
    float m_ttl;
    int m_numPoints;
	Vector2f m_centerCoordinate;
    float m_radiansPerSec;
    float m_vx;
    float m_vy;
    View m_cartesianPlane;
    Color m_color1;
    Color m_color2;
    Matrix m_A;

    /* EXTRA */
    GameSprite m_sprite;
    bool m_bUseSprite = false;
    bool m_bIsRareBullet = false;
};
