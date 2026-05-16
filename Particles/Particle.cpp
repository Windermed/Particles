#include "Particle.h"
#include "Engine.h"


Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition, Engine* owningEngine) : m_A(2, numPoints), m_engine(owningEngine)
{
    this->m_ttl = TTL;
    this->m_numPoints = numPoints;
    this->m_radiansPerSec = ((float)rand() / RAND_MAX) * M_PI;

    this->m_cartesianPlane.setCenter(0, 0);
    this->m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);

    // maps the mouse click pos to Cartesian coords.
    this->m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    this->m_vx = (float)(rand() % 401 + 100);

    if (rand() % 2 != 0)
    {
        this->m_vx *= -1;
    }

    this->m_vy = (float)(rand() % 401 + 100);

    // Colors
    this->m_color1 = Color::White;
    this->m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

    // starts generating numPoint vertices
    float theta = ((float)rand() / RAND_MAX) * (M_PI / 2.0f);
    float dTheta = 2.0f * M_PI / (numPoints - 1);

    for (int j = 0; j < numPoints; j++)
    {
        float r = (float)(rand() % 80 + 20);

        float dx = r * cos(theta);
        float dy = r * sin(theta);

        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;

        theta += dTheta;
    }

}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    VertexArray lines(TriangleFan, m_numPoints + 1);
    
    // Maps Cartesian Coordinates to screen coords. it will then translate the origin to the center of screen
    // after that, it will flip the y-axis so positive y will point upwards.
    Vector2u size = target.getSize();
    Transform transform;
    transform.translate(size.x / 2.0f, size.y / 2.0f);
    transform.scale(1.0f, -1.0f);
    states.transform = transform;

    // Use Cartesian coordinates directly
    lines[0].position = m_centerCoordinate;
    lines[0].color = m_color1;

    // assigns each outer vertex its cartesian position from m_A and applies the particle's secondary color.
    for(int j = 1; j <= m_numPoints; j++)
    {
        
        lines[j].position = Vector2f((float)m_A(0, j - 1), (float)m_A(1, j - 1));
        lines[j].color = m_color2;
    }

    // draw
    target.draw(lines, states);
}

void Particle::Update(float dt)
{
    m_ttl -= dt;
    rotate(dt * m_radiansPerSec);

    // checks to see if particle has scaling enabled.
    if (m_shouldScale)
    {
        scale(SCALE);
    }
    

    float dx = m_vx * dt;

    // checks to see if the engine flag for zero gravity is on or off.
    
    bool bIsGravityActive = m_UseGravity && !m_engine->m_bIsZeroGravityOn;
    /*
    if (m_engine->m_bIsZeroGravityOn)
    {
        m_vy -= 0 * dt;
    }
    else 
    {
        m_vy -= G * dt; // you can also set to 0 for zero gravity.
    }
    */

    m_vy -= bIsGravityActive ? G * dt : 0.0f;

    
    float dy = m_vy * dt;

    translate(dx, dy);
}


bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.GetRows() == 2 && r.GetCols() == 2 && almostEqual(r(0, 0), cos(theta))
    && almostEqual(r(0, 1), -sin(theta))
    && almostEqual(r(1, 0), sin(theta))
    && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.GetRows() == 2 && s.GetCols() == 2 && almostEqual(s(0, 0), 1.5)
    && almostEqual(s(0, 1), 0)
    && almostEqual(s(1, 0), 0)
    && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.GetRows() == 2 && t.GetCols() == 3 && almostEqual(t(0, 0), 5) 
        && almostEqual(t(1, 0), -5) 
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;

    for (int j = 0; j < initialCoords.GetCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }

    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;

    for (int j = 0; j < initialCoords.GetCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }

    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;

    for (int j = 0; j < initialCoords.GetCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }

    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

bool Particle::IsOffScreen()
{
    Vector2u size = m_engine->GetWindow().getSize();
    Vector2f center = this->GetCenter();

    return center.x < -(size.x / 2.0f) || center.x >(size.x / 2.0f) || center.y < -(size.y / 2.0f) || center.y >(size.y / 2.0f);
}

// returns the furthest vertex distance from center
// which will essentially give us the particle's effective collision radius.
float Particle::GetBoundingRadius() const
{
    float maxDist = 0.0f;
    for (int j = 0; j < m_numPoints; j++)
    {
        float dx = (float)m_A(0, j) - m_centerCoordinate.x;
        float dy = (float)m_A(1, j) - m_centerCoordinate.y;
        float dist = sqrt(dx * dx + dy * dy);
        maxDist = max(maxDist, dist);

    }
    return maxDist;
}


void Particle::rotate(double theta)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    RotationMatrix R(theta);
    m_A = R * m_A; // no need to use this.
    translate(temp.x, temp.y);
}

// scales all vertices relative to the particle center
void Particle::scale(double c)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    ScalingMatrix S(c);
    m_A = S * m_A;
    translate(temp.x, temp.y);
}


// shifts vertices and center coords by xShift and yShift
void Particle::translate(double xShift, double yShift)
{
    TranslationMatrix T(xShift, yShift, m_numPoints);
    m_A = T + m_A;

    m_centerCoordinate.x += (float)xShift;
    m_centerCoordinate.y += (float)yShift;
}