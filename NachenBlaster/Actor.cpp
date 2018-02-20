#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth)
:GraphObject(imageID, startX, startY, dir, size, depth), m_hitpoints(0)
{ }

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
void Actor::collisionReaction()
{
    return;
}

bool Actor::die()
{
    if (m_hitpoints == 0)
    {
        //getWorld()->cleanUp();
        return true;
    }
    else
        return false;
}

void Actor::setHitpoints(int hp)
{
    m_hitpoints = hp;
}

bool Actor::offScreen()
{
    if (getX() < 0 || getX() > 255)
        return true;
    if (getY() < 0 || getY() > 255)
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

NachenBlaster::NachenBlaster()
:Actor(IID_NACHENBLASTER, 0, 128), m_nCabbage(30), m_nTorpedo(0)
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
                        //animate();
                        m_nCabbage -= 5;
                    }
                    break;
                case KEY_PRESS_TAB:
                    if (m_nTorpedo > 0)
                    {
                        //animate();
                        m_nTorpedo--;
                    }
                    break;
                case KEY_PRESS_UP:
                    if (!offScreen())
                        moveTo(getX(), getY()+6);
                    break;
                case KEY_PRESS_DOWN:
                    if (!offScreen())
                        moveTo(getX(), getY()-6);
                    break;
                case KEY_PRESS_LEFT:
                    if (!offScreen())
                        moveTo(getX()-6, getY());
                    break;
                case KEY_PRESS_RIGHT:
                    if (!offScreen())
                        moveTo(getX()+6, getY());
                    break;
            }
        }
        if (m_nCabbage < 30)
            m_nCabbage++;
    }
}
NachenBlaster::~NachenBlaster()
{
    std::cerr << "NachenBlaster Destructor" << std::endl;
}

Star::Star(int x, int y, int r)
:Actor(IID_STAR, x, y, 0, r, 3)
{ }

void Star::doSomething()
{
    int x = getX()-1;
    moveTo(x, getY());
    if (getX() < 0)
        setHitpoints(0);
}

Star::~Star()
{
    std::cerr << "Star Destructor" << std::endl;
}

