#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir, double size, int depth)
:GraphObject(imageID, startX, startY, dir, size, depth), m_hitpoints(50)
{
    m_world = sw;
}

double Actor::dist(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

bool Actor::collide(int x1, int y1, int r1, int x2, int y2, int r2)
{
    if (dist(x1, x2, y1, y2) < 0.75 * (r1 + r2))
        return true;
    else
        return false;
}
void Actor::sufferDamage()
{
    m_hitpoints -= 10;
}

bool Actor::die()
{
    return (m_hitpoints == 0);
}

void Actor::setHitpoints(int hp)
{
    m_hitpoints = hp;
}

void Actor::setDead()
{
    m_hitpoints = 0;
}
int Actor::getHitpoints()
{
    return m_hitpoints;
}

bool Actor::offScreen(int x, int y)
{
    if (x < 0 || x > 255)
        return true;
    if (y < 0 || y > 255)
        return true;
    return false;
}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

Actor::~Actor()
{
    std::cerr << "Actor Destructor" << std::endl;
}

NachenBlaster::NachenBlaster(StudentWorld* sw)
:Actor(sw, IID_NACHENBLASTER, 0, 128), m_nCabbage(30), m_nTorpedo(0)
{
    setHitpoints(50);
}

void NachenBlaster::doSomething()
{
    if (die())
        return;
    else
    {
        int ch;
        if (getWorld()->getKey(ch))
        {
            switch(ch)
            {
                case KEY_PRESS_SPACE:
                    if (m_nCabbage >= 5)
                    {
                        for (int i = 0; i < 5; i++)
                        {
                            Cabbage* c = new Cabbage(getWorld(), getX()+12, getY());
                            getWorld()->animate(c);
                        }
                        m_nCabbage -= 5;
                    }
                    break;
                case KEY_PRESS_TAB:
                    if (m_nTorpedo > 0)
                    {
                        Torpedo* t = new Torpedo(getWorld(), getX()+12, getY(), 0);
                        getWorld()->animate(t);
                        m_nTorpedo--;
                    }
                    break;
                case KEY_PRESS_UP:
                    if (!offScreen(getX(), getY()+6))
                        moveTo(getX(), getY()+6);
                    break;
                case KEY_PRESS_DOWN:
                    if (!offScreen(getX(), getY()-6))
                        moveTo(getX(), getY()-6);
                    break;
                case KEY_PRESS_LEFT:
                    if (!offScreen(getX()-6, getY()))
                        moveTo(getX()-6, getY());
                    break;
                case KEY_PRESS_RIGHT:
                    if (!offScreen(getX()+6, getY()))
                        moveTo(getX()+6, getY());
                    break;
            }
        }
        if (m_nCabbage < 30)
            m_nCabbage++;
    }
}
void NachenBlaster::sufferDamage()
{
    setHitpoints(getHitpoints()-10);
}

int NachenBlaster::getCabbage()
{
    return m_nCabbage;
}
int NachenBlaster::getTorpedo()
{
    return m_nTorpedo;
}

NachenBlaster::~NachenBlaster()
{
    std::cerr << "NachenBlaster Destructor" << std::endl;
}

Star::Star(StudentWorld* sw, int x, int y, double r)
:Actor(sw, IID_STAR, x, y, 0, r, 3)
{ }

void Star::doSomething()
{
    int x = getX()-1;
    moveTo(x, getY());
    if (offScreen(getX(), getY()))
        setDead();
}

Star::~Star()
{
    std::cerr << "Star Destructor" << std::endl;
}

Projectile::Projectile(StudentWorld* sw, int imageID, double startX, double startY, int dir, double size, int depth)
:Actor(sw, imageID, startX, startY, dir, size, depth)
{ }

void Projectile::doSomething()
{
    if (die())
        return;
    if (offScreen(getX(), getY()))
        setDead();
}

Projectile::~Projectile()
{
    std::cerr << "Projectile Destructor" << std::endl;
}

Cabbage::Cabbage(StudentWorld* sw, int x, int y)
:Projectile(sw, IID_CABBAGE, x, y, 0, 0.5, 1)
{ }

void Cabbage::doSomething()
{
    Projectile::doSomething();
    if (!die())
    {
        moveTo(getX()+8, getY());
        setDirection(getDirection()+20);
    }
}
Turnip::Turnip(StudentWorld* sw, int x, int y)
:Projectile(sw, IID_TURNIP, x, y, 0, 0.5, 1)
{ }

void Turnip::doSomething()
{
    Projectile::doSomething();
    if (!die())
    {
        moveTo(getX()-6, getY());
        setDirection(getDirection()+20);
    }
}

Torpedo::Torpedo(StudentWorld* sw, int x, int y, int d)
:Projectile(sw, IID_TORPEDO, x, y, d, 0.5, 1)
{ }

void Torpedo::doSomething()
{
    Projectile::doSomething();
    if (!die())
    {
        moveTo(getX()+6, getY());
    }
}
