#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


/////////////////////////////////////////////////////////////////////////
/////////////////////////////   Actor    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir, double size, int depth)
:GraphObject(imageID, startX, startY, dir, size, depth), m_hitpoints(50)
{
    m_world = sw;
}

void Actor::doSomething()
{
    if (die())
        return;
    if (offScreen(getX(), getY()))
        setDead();
}

bool Actor::isAlien()
{
    return false;
}

double Actor::dist(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

bool Actor::collide(Actor* obj)
{
    int x1 = getX();
    int y1 = getY();
    int r1 = getRadius();
    int x2 = obj->getX();
    int y2 = obj->getY();
    int r2 = obj->getRadius();
    if (dist(x1, x2, y1, y2) < 0.75 * (r1 + r2))
        return true;
    else
        return false;
}

void Actor::collisionReaction()
{
    return;
}

void Actor::sufferDamage(int d)
{
    m_hitpoints -= d;
}

bool Actor::die()
{
    return (m_hitpoints == 0);
}

void Actor::setHitpoints(double hp)
{
    m_hitpoints = hp;
}

void Actor::setDead()
{
    m_hitpoints = 0;
}

double Actor::getHitpoints()
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


/////////////////////////////////////////////////////////////////////////
////////////////////////////NachenBlaster////////////////////////////////
/////////////////////////////////////////////////////////////////////////

NachenBlaster::NachenBlaster(StudentWorld* sw)
:Actor(sw, IID_NACHENBLASTER, 0, 128), m_nCabbage(30), m_nTorpedo(0)
{
    setHitpoints(50);
}

bool NachenBlaster::collidableWithNB()
{
    return false;
}

bool NachenBlaster::collidableWithAlien()
{
    return true;
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


/////////////////////////////////////////////////////////////////////////
/////////////////////////////   Alien    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////

Alien::Alien(StudentWorld* sw, int imageID, double startX, double startY, int dir, double size, int depth)
:Actor(sw, imageID, startX, startY, dir, size, depth), m_speed(2.0), m_length(VIEW_WIDTH)
{
    int randDir = randInt(1, 3);
    setTravelDir(randDir);
    int randL = randInt(1, 32);
    setLength(randL);
}

bool Alien::isAlien()
{
    return true;
}

bool Alien::collidableWithNB()
{
    return true;
}

bool Alien::collidableWithAlien()
{
    return false;
}

void Alien::doSomething()
{
    if (die())
        return;
    if (getX() <= 0)
        setDead();
    // check collision
    if (getWorld()->collide())
    {
        collisionReaction();
    }
    // if need new flight plan
    if (getY() <= 0 || getY() >= VIEW_WIDTH-1 || getLength() <= 0)
        changeDir();
    
    // if can fire
    int nbX = getWorld()->getPlayer()->getX();
    int nbY = getWorld()->getPlayer()->getY();
    if (getX() > nbX && getY() <= nbY + 4 && getY() >= nbY - 4)
        fire();
    
    // continue moving
    move();
    
    //check collision
    if (getWorld()->collide())
    {
        collisionReaction();
    }
}

void Alien::changeDir()
{
    if (getY() >= VIEW_WIDTH-1)
    {
        setTravelDir(DOWNLEFT);
    }
    else if (getY() <= 0)
    {
        setTravelDir(UPLEFT);
    }
    changeDirDiff();
}

void Alien::fire()
{
    //fireDiff();
    getWorld()->playSound(SOUND_ALIEN_SHOOT);
}

void Alien::move()
{
    double n = getSpeed();
    // upleft
    //moveTo(getX()-n, getY()+n);
    // downleft
    //moveTo(getX()-n, getY()-n);
    // due left
    
    switch (getTravelDir()) {
        case UPLEFT:
            moveTo(getX()-n, getY()+n);
            break;
        case DOWNLEFT:
            moveTo(getX()-n, getY()-n);
            break;
        case DUELEFT:
            moveTo(getX()-n, getY());
        default:
            break;
    }
    moveDiff();
}

void Alien::collisionReaction()
{
    // if (collide with NB proj)
    sufferDamage(0);
    /*if (getHitpoints() <= 0)
    {
        getWorld()->increaseScore(0);
        setDead();
        getWorld()->playSound(SOUND_DEATH);
        Explosion* ex = new Explosion(getWorld(), getX(), getY());
        getWorld()->animate(ex);
    }
    */
    
    //if (getWorld()->collide())
    //{
        getWorld()->getPlayer()->sufferDamage(0);
        setDead();
        getWorld()->playSound(SOUND_DEATH);
        Explosion* ex = new Explosion(getWorld(), getX(), getY());
        getWorld()->animate(ex);
        dropGoodie();
    //}
}

double Alien::getSpeed()
{
    return m_speed;
}

int Alien::getLength()
{
    return m_length;
}

int Alien::getTravelDir()
{
    return m_travelDir;
}

void Alien::setSpeed(double s)
{
    m_speed = s;
}

void Alien::setLength(int l)
{
    m_length = l;
}

void Alien::setTravelDir(int tr)
{
    m_travelDir = tr;
}

//////////////////////////Smallgon////////////////////////////////
Smallgon::Smallgon(StudentWorld* sw, double startX, double startY)
:Alien(sw, IID_SMALLGON, startX, startY)
{
    int n = getWorld()->getLevel();
    setHitpoints(5 * (1 + (n-1) * 0.1));
}

void Smallgon::doSomething()
{
    Alien::doSomething();
}

void Smallgon::changeDirDiff()
{
    if (getLength() <= 0)
    {
        int randdir = randInt(1, 3);
        switch(randdir)
        {
            case 1:
                // upleft
                setTravelDir(UPLEFT);
                break;
            case 2:
                // downleft
                setTravelDir(DOWNLEFT);
                break;
            case 3:
                // due left
                setTravelDir(DUELEFT);
                break;
        }
    }
    int randl = randInt(1, 32);
    setLength(randl);
}

void Smallgon::fireDiff()
{
    Turnip* t = new Turnip(getWorld(), getX()-14, getY());
    getWorld()->animate(t);
}

void Smallgon::moveDiff()
{
    setLength(getLength()-1);
}

void Smallgon::dropGoodie()
{
    return;
}
//////////////////////////Smoregon//////////////////////////////////
Smoregon::Smoregon(StudentWorld* sw, double startX, double startY)
:Alien(sw, IID_SMOREGON, startX, startY)
{
    int n = getWorld()->getLevel();
    setHitpoints(5 * (1 + (n-1) * 0.1));
}

void Smoregon::doSomething()
{
    Alien::doSomething();
}
void Smoregon::changeDirDiff()
{
    if (getLength() <= 0)
    {
        int randdir = randInt(1, 3);
        //int n = getWorld()->getLevel();
        switch(randdir)
        {
            case 1:
                // upleft
                setTravelDir(UPLEFT);
                break;
            case 2:
                // downleft
                setTravelDir(DOWNLEFT);
                break;
            case 3:
                // due left
                setTravelDir(DUELEFT);
                break;
        }
    }
    int randl = randInt(1, 32);
    setLength(randl);
}

void Smoregon::fireDiff()
{
    Turnip* t = new Turnip(getWorld(), getX()-14, getY());
    getWorld()->animate(t);
}

void Smoregon::moveDiff()
{
    setLength(getLength()-1);
}

void Smoregon::dropGoodie()
{
    return;
}
//////////////////////////Snagglegon////////////////////////////////
Snagglegon::Snagglegon(StudentWorld* sw, double startX, double startY, int travelDir)
:Alien(sw, IID_SNAGGLEGON, startX, startY)
{
    int n = getWorld()->getLevel();
    setHitpoints(10 * (1 + (n-1) * 0.1));
    setTravelDir(DOWNLEFT);
    setSpeed(1.75);
}

void Snagglegon::doSomething()
{
    Alien::doSomething();
}

void Snagglegon::changeDirDiff()
{
    int randdir = randInt(1, 2);
    double n = getSpeed();
    switch(randdir)
    {
        case 1:
            // upleft
            moveTo(getX()-n, getY()+n);
            break;
        case 2:
            // downleft
            moveTo(getX()-n, getY()-n);
            break;
    }
}

void Snagglegon::fireDiff()
{
    Torpedo* t = new Torpedo(getWorld(), getX()-14, getY(), 0);
    getWorld()->animate(t);
}

void Snagglegon::moveDiff()
{
    return;
}

void Snagglegon::dropGoodie()
{
    return;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////    Star    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////

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

bool Star::collidableWithNB()
{
    return false;
}
bool Star::collidableWithAlien()
{
    return false;
}

Star::~Star()
{
    std::cerr << "Star Destructor" << std::endl;
}


/////////////////////////////////////////////////////////////////////////
///////////////////////////// Projectile ////////////////////////////////
/////////////////////////////////////////////////////////////////////////

Projectile::Projectile(StudentWorld* sw, int imageID, double startX, double startY, int dir, double size, int depth)
:Actor(sw, imageID, startX, startY, dir, size, depth)
{ }

Projectile::~Projectile()
{
    std::cerr << "Projectile Destructor" << std::endl;
}

Cabbage::Cabbage(StudentWorld* sw, int x, int y)
:Projectile(sw, IID_CABBAGE, x, y, 0, 0.5, 1)
{ }

bool Cabbage::collidableWithNB()
{
    return false;
}

bool Cabbage::collidableWithAlien()
{
    return true;
}

void Cabbage::doSomething()
{
    Actor::doSomething();
    if (!die())
    {
        moveTo(getX()+8, getY());
        setDirection(getDirection()+20);
    }
}
Turnip::Turnip(StudentWorld* sw, int x, int y)
:Projectile(sw, IID_TURNIP, x, y, 0, 0.5, 1)
{ }

bool Turnip::collidableWithNB()
{
    return true;
}

bool Turnip::collidableWithAlien()
{
    return false;
}

void Turnip::doSomething()
{
    Actor::doSomething();
    if (!die())
    {
        moveTo(getX()-6, getY());
        setDirection(getDirection()+20);
    }
}

Torpedo::Torpedo(StudentWorld* sw, int x, int y, int d)
:Projectile(sw, IID_TORPEDO, x, y, d, 0.5, 1)
{ }

bool Torpedo::collidableWithNB()
{
    return true;
}

bool Torpedo::collidableWithAlien()
{
    return true;
}

void Torpedo::doSomething()
{
    Actor::doSomething();
    if (!die())
    {
        moveTo(getX()+6, getY());
    }
}

/////////////////////////////////////////////////////////////////////////
///////////////////////////// Explosion  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Explosion::Explosion(StudentWorld* sw, int startX, int startY)
:Actor(sw, IID_EXPLOSION, startX, startY, 0, 0.5, 0), m_tick(0)
{ }

bool Explosion::collidableWithNB()
{
    return false;
}
bool Explosion::collidableWithAlien()
{
    return false;
}

int Explosion::getTickCount()
{
    return m_tick;
}

void Explosion::doSomething()
{
    setSize(1.5 * getSize());
    m_tick++;
    if (getTickCount() > 4)
        setDead();
}
