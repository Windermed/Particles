#include "Particle.h"

// TODO: DO SOMETHING EXTRA.
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
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

    // Build transform: convert Cartesian to pixel coords
    // pixel_x = Cartesian_x + W/2
    // pixel_y = H/2 - Cartesian_y
    Vector2u size = target.getSize();
    Transform transform;
    transform.translate(size.x / 2.0f, size.y / 2.0f);
    transform.scale(1.0f, -1.0f);
    states.transform = transform;

    // Use Cartesian coordinates directly
    lines[0].position = m_centerCoordinate;
    lines[0].color = m_color1;

    for (int j = 1; j <= m_numPoints; j++)
    {
        
        lines[j].position = Vector2f((float)m_A(0, j - 1), (float)m_A(1, j - 1));

        float ratio = m_ttl / TTL;
        lines[j].color = Color((Uint8)(m_color2.r * ratio), (Uint8)(m_color2.g * ratio), (Uint8)(m_color2.b * ratio));
    }

    target.draw(lines, states);
}

void Particle::Update(float dt)
{
    this->m_ttl -= dt;
    rotate(dt * m_radiansPerSec);
    scale(SCALE);

    float dx = m_vx * dt;
    m_vy -= G * dt; // you can also set to 0 for zero gravity.
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

void Particle::rotate(double theta)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    RotationMatrix R(theta);
    this->m_A = R * this->m_A;
    translate(temp.x, temp.y);
}

void Particle::scale(double c)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    ScalingMatrix S(c);
    this->m_A = S * this->m_A;
    translate(temp.x, temp.y);
}

void Particle::translate(double xShift, double yShift)
{
    TranslationMatrix T(xShift, yShift, m_numPoints);
    this->m_A = T + m_A;

    m_centerCoordinate.x += (float)xShift;
    m_centerCoordinate.y += (float)yShift;
}