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
    else
        doSomethingDiff();
}

bool Actor::isAlien()
{
    return false;
}

double Actor::dist(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}


void Actor::sufferDamage(int d)
{
    m_hitpoints -= d;
}

bool Actor::die()
{
    return m_hitpoints == 0;
}

void Actor::setHitpoints(double hp)
{
    m_hitpoints = hp;
}

void Actor::setDead()
{
    m_hitpoints = 0;
}

int Actor::getLabel()
{
    return m_label;
}

void Actor::setLabel(int n)
{
    m_label = n;
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

bool Actor::collideNB()
{
    int xp = getWorld()->getPlayer()->getX();
    int yp = getWorld()->getPlayer()->getY();
    int rp = getWorld()->getPlayer()->getRadius();
    int x = getX();
    int y = getY();
    int r = getRadius();
    return (dist(x, y, xp, yp) < 0.75 * (r + rp));
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
    setLabel(PLAYER);
}

void NachenBlaster::doSomethingDiff()
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

void NachenBlaster::sufferDamage(int d)
{
    setHitpoints(getHitpoints()-d);
    if (getHitpoints() <= 0)
        setDead();
    getWorld()->playSound(SOUND_BLAST);
}

int NachenBlaster::getCabbage()
{
    return m_nCabbage;
}

void NachenBlaster::addTorpedo(int n)
{
    m_nTorpedo += n;
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
    setLabel(ENEMY);
}

bool Alien::isAlien()
{
    return true;
}

void Alien::dropGoodie()
{
    return;
}

void Alien::sufferDamage(int d)
{
    setHitpoints(getHitpoints()-d);
    if (getHitpoints() <= 0)
    {
        setDead();
        getWorld()->playSound(SOUND_DEATH);
        Explosion* ex = new Explosion(getWorld(), getX(), getY());
        getWorld()->animate(ex);
        dropGoodie();
    }
    else
        getWorld()->playSound(SOUND_BLAST);
}
void Alien::doSomethingDiff()
{
    if (getX() <= 0)
        setDead();
    
    checkCollision();
    
    // if need new flight plan
    if (getY() <= 0 || getY() >= VIEW_WIDTH-1 || getLength() <= 0)
        changeDir();
    
    // if can fire
    int nbX = getWorld()->getPlayer()->getX();
    int nbY = getWorld()->getPlayer()->getY();
    if (getX() > nbX && getY() <= nbY + 4 && getY() >= nbY - 4)
        fire();
    
    move();

    checkCollision();
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

void Alien::fatalCollision(int score)
{
    getWorld()->increaseScore(score);
    setDead();
    getWorld()->playSound(SOUND_DEATH);
    Explosion* ex = new Explosion(getWorld(), getX(), getY());
    getWorld()->animate(ex);
}

Alien::~Alien()
{
    std::cerr << "Alien Destructor" << std::endl;
}

//////////////////////////Smallgon////////////////////////////////
Smallgon::Smallgon(StudentWorld* sw, double startX, double startY)
:Alien(sw, IID_SMALLGON, startX, startY)
{
    int n = getWorld()->getLevel();
    setHitpoints(5 * (1 + (n-1) * 0.1));
}

void Smallgon::changeDirDiff()
{
    if (getLength() <= 0)
    {
        int randdir = randInt(1, 3);
        switch(randdir)
        {
            case 1:
                setTravelDir(UPLEFT);
                break;
            case 2:
                setTravelDir(DOWNLEFT);
                break;
            case 3:
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

void Smallgon::checkCollision()
{
    if (collideNB())
    {
        getWorld()->getPlayer()->sufferDamage(5);
        fatalCollision(250);
    }
    else if (getWorld()->collide(this))
    {
        if (die())
        {
            fatalCollision(250);
        }
        else
            getWorld()->playSound(SOUND_BLAST);
    }
}

//////////////////////////Smoregon//////////////////////////////////
Smoregon::Smoregon(StudentWorld* sw, double startX, double startY)
:Alien(sw, IID_SMOREGON, startX, startY)
{
    int n = getWorld()->getLevel();
    setHitpoints(5 * (1 + (n-1) * 0.1));
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

void Smoregon::checkCollision()
{
    if (collideNB())
    {
        getWorld()->getPlayer()->sufferDamage(5);
        fatalCollision(250);
        dropGoodie();
    }
    else if (getWorld()->collide(this))
    {
        if (die())
        {
            fatalCollision(250);
            dropGoodie();
        }
        else
            getWorld()->playSound(SOUND_BLAST);
    }
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
    Torpedo* t = new Torpedo(getWorld(), getX()-14, getY(), 180);
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

void Snagglegon::checkCollision()
{
    if (collideNB())
    {
        getWorld()->getPlayer()->sufferDamage(15);
        fatalCollision(1000);
        dropGoodie();
    }
    else if (getWorld()->collide(this))
    {
        if (die())
        {
            fatalCollision(1000);
            dropGoodie();
        }
        else
            getWorld()->playSound(SOUND_BLAST);
    }
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////    Star    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Star::Star(StudentWorld* sw, int x, int y)
:Actor(sw, IID_STAR, x, y, 0, 0, 3)
{
    double randomSize = randInt(5, 50) / 100.0;
    setSize(randomSize);
    setLabel(NEUTRAL);
}

void Star::doSomethingDiff()
{
    int x = getX()-1;
    moveTo(x, getY());
    if (getX() < 0)
        setDead();
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

 void Projectile::doSomethingDiff()
{
    if (getX() < 0 || getX() >= VIEW_WIDTH)
        setDead();
}

Projectile::~Projectile()
{
    std::cerr << "Projectile Destructor" << std::endl;
}


///////////////////////////////Cabbage//////////////////////////////////
Cabbage::Cabbage(StudentWorld* sw, int x, int y)
:Projectile(sw, IID_CABBAGE, x, y, 0, 0.5, 1)
{
    setLabel(PLAYER);
}

void Cabbage::doSomethingDiff()
{
    // if collide with an alien
    // setDead();
    // alien.sufferDamage(2);
    
    if (!die())
    {
        moveTo(getX()+8, getY());
        setDirection(getDirection()+20);
    }
    
    // if collide with an alien
    // setDead();
    // alien.sufferDamage(2);
}

void Cabbage::checkCollision()
{
    if (getWorld()->collide(this))
    {
        setDead();
        // how should i make the alien ship suffer damage?
    }
}

//////////////////////////////Turnip///////////////////////////////////
Turnip::Turnip(StudentWorld* sw, int x, int y)
:Projectile(sw, IID_TURNIP, x, y, 0, 0.5, 1)
{
    setLabel(ENEMY);
}

void Turnip::doSomethingDiff()
{
    checkCollision();
    if (!die())
    {
        moveTo(getX()-6, getY());
        setDirection(getDirection()+20);
    }
    checkCollision();
}

void Turnip::checkCollision()
{
    if (collideNB())
    {
        setDead();
        getWorld()->getPlayer()->sufferDamage(2);
    }
}

///////////////////////////////Torpedo//////////////////////////////////
Torpedo::Torpedo(StudentWorld* sw, int x, int y, int d)
:Projectile(sw, IID_TORPEDO, x, y, d, 0.5, 1)
{
    setDirection(d);
    // give it a label when fire()
}

void Torpedo::doSomethingDiff()
{
    checkCollision();
    if (!die())
        moveTo(getX()+6, getY());
    checkCollision();
}

void Torpedo::checkCollision()
{
    if (getWorld()->collide(this))
    {
        setDead();
        // how should i make the alien ship suffer damage?
    }
    if (collideNB())
    {
        setDead();
        getWorld()->getPlayer()->sufferDamage(8);
    }
}

/////////////////////////////////////////////////////////////////////////
///////////////////////////// Explosion  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Explosion::Explosion(StudentWorld* sw, int startX, int startY)
:Actor(sw, IID_EXPLOSION, startX, startY, 0, 0.5, 0), m_tick(0)
{
    setLabel(NEUTRAL);
}

int Explosion::getTickCount()
{
    return m_tick;
}

void Explosion::doSomethingDiff()
{
    setSize(1.5 * getSize());
    m_tick++;
    if (getTickCount() > 4)
        setDead();
}

/////////////////////////////////////////////////////////////////////////
//////////////////////////////// Goodie  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Goodie::Goodie(StudentWorld* sw, int imageID, int x, int y)
:Actor(sw, imageID, x, y, 0, 0.5, 1)
{
    setLabel(NEUTRAL);
}

void Goodie::doSomethingDiff()
{
    if (collideNB())
    {
        collisionReaction();
        return;
    }
    moveTo(getX()-0.75, getY()-0.75);
    if (collideNB())
    {
        collisionReaction();
        return;
    }
}

void Goodie::collisionReaction()
{
    getWorld()->increaseScore(100);
    setDead();
    getWorld()->playSound(SOUND_GOODIE);
    bonus();
}

////////////////////////////////  Extralife  ////////////////////////////////
Extralife::Extralife(StudentWorld* sw, int startX, int startY)
: Goodie(sw, IID_LIFE_GOODIE, startX, startY)
{ }

void Extralife::bonus()
{
    getWorld()->incLives();
}
////////////////////////////////   Repair    ////////////////////////////////
Repair::Repair(StudentWorld* sw, int startX, int startY)
: Goodie(sw, IID_REPAIR_GOODIE, startX, startY)
{ }

void Repair::bonus()
{
    getWorld()->getPlayer()->setHitpoints(getHitpoints() + 10);
}
////////////////////////////////TorpedoGoodie////////////////////////////////
TorpedoGoodie::TorpedoGoodie(StudentWorld* sw, int startX, int startY)
: Goodie(sw, IID_TORPEDO_GOODIE, startX, startY)
{ }

void TorpedoGoodie::bonus()
{
    getWorld()->getPlayer()->addTorpedo(5);
}
