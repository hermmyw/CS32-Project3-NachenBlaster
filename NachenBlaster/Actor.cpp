#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

/////////////////////   Helper Function  /////////////////////
double dist(double x1, double y1, double x2, double y2)
{
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////   Actor    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Actor::Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir, double size, int depth)
:GraphObject(imageID, startX, startY, dir, size, depth), m_hitpoints(FULLHITPOINT)
{
    m_world = sw;
}

void Actor::doSomething()
{
    if (dead())
        return;
    else
        doSomethingDiff();
}

void Actor::sufferDamage(double d)
{
    m_hitpoints -= d;
}

bool Actor::alienShip() const
{
    return false;
}

int Actor::getDamagePoints() const
{
    return 0;
}

double Actor::getHitpoints() const
{
    return m_hitpoints;
}

int Actor::getLabel() const
{
    return m_label;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}


void Actor::setHitpoints(double hp)
{
    m_hitpoints = hp;
}

void Actor::setDead()
{
    m_hitpoints = 0.0;
}

void Actor::setLabel(int n)
{
    m_label = n;
}

bool Actor::dead() const
{
    return m_hitpoints <= 0;
}

bool Actor::offScreen(double x, double y) const
{
    if (x < 0 || x > VIEW_WIDTH-1)
        return true;
    else if (y < 0 || y > VIEW_HEIGHT-1)
        return true;
    else
        return false;
}

// Check if an actor collides with the NachenBlaster
bool Actor::collideNB() const
{
    double xp = getWorld()->getPlayer()->getX();
    double yp = getWorld()->getPlayer()->getY();
    double rp = getWorld()->getPlayer()->getRadius();
    double x = getX();
    double y = getY();
    double r = getRadius();
    return (dist(x, y, xp, yp) < 0.75 * (r + rp));
}

Actor::~Actor()
{
    //std::cerr << "Actor Destructor" << std::endl;
}


/////////////////////////////////////////////////////////////////////////
////////////////////////////NachenBlaster////////////////////////////////
/////////////////////////////////////////////////////////////////////////

NachenBlaster::NachenBlaster(StudentWorld* sw)
:Actor(sw, IID_NACHENBLASTER, 0, VIEW_HEIGHT/2), m_nCabbage(FULLCABBAGE), m_nTorpedo(0)
{
    setHitpoints(FULLHITPOINT);
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
                        getWorld()->playSound(SOUND_PLAYER_SHOOT);
                    }
                    m_nCabbage -= 5;
                }
                break;
            case KEY_PRESS_TAB:
                if (m_nTorpedo > 0)
                {
                    Torpedo* t = new Torpedo(getWorld(), getX()+12, getY(), 0);
                    t->setLabel(PLAYER);
                    getWorld()->animate(t);
                    getWorld()->playSound(SOUND_TORPEDO);
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
    if (m_nCabbage < FULLCABBAGE)
        m_nCabbage++;
}

void NachenBlaster::sufferDamage(double d)
{
    setHitpoints(getHitpoints()-d);
    if (getHitpoints() <= 0)
        setDead();
}

int NachenBlaster::getCabbage() const
{
    return m_nCabbage;
}

void NachenBlaster::addTorpedo(int n)
{
    m_nTorpedo += n;
}

int NachenBlaster::getTorpedo() const
{
    return m_nTorpedo;
}

NachenBlaster::~NachenBlaster()
{
    //std::cerr << "NachenBlaster Destructor" << std::endl;
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////    Star    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Star::Star(StudentWorld* sw, double startX, double startY)
:Actor(sw, IID_STAR, startX, startY, 0, 0, 3)
{
    double randomSize = randInt(5, 50) / 100.0;
    setSize(randomSize);
    setLabel(NEUTRAL);
}

void Star::doSomethingDiff()
{
    // The star must move 1 pixel to the left.
    int x = getX()-1;
    moveTo(x, getY());
    if (getX() < 0)
        setDead();
}

Star::~Star()
{
    //std::cerr << "Star Destructor" << std::endl;
}

/////////////////////////////////////////////////////////////////////////
///////////////////////////// Explosion  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Explosion::Explosion(StudentWorld* sw, double startX, double startY)
:Actor(sw, IID_EXPLOSION, startX, startY, 0, 0.5, 0), m_tick(0)
{
    setLabel(NEUTRAL);
}

void Explosion::doSomethingDiff()
{
    // is it tooooo big?
    setSize(1.5 * getSize());
    m_tick++;
    if (m_tick > 4)
        setDead();
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
    else
    {
        checkCollision();
        if (!dead())
        {
            moveProjectile();
            checkCollision();
        }
    }
}

void Projectile::sufferDamage(double d)
{
    // Projectiles do not suffer damage.
    return;
}

Projectile::~Projectile()
{
    //std::cerr << "Projectile Destructor" << std::endl;
}


///////////////////////////////Cabbage//////////////////////////////////
Cabbage::Cabbage(StudentWorld* sw, double startX, double startY)
:Projectile(sw, IID_CABBAGE, startX, startY, 0, 0.5, 1)
{
    setLabel(PLAYER);
}

void Cabbage::moveProjectile()
{
    moveTo(getX()+8, getY());
    setDirection(getDirection()+20);
}

void Cabbage::checkCollision()
{
    double damage = 0.0;
    if (getWorld()->collide(this, damage))
        setDead();
}

int Cabbage::getDamagePoints() const
{
    return CABBAGEDAMAGE;
}

//////////////////////////////Turnip///////////////////////////////////
Turnip::Turnip(StudentWorld* sw, double startX, double startY)
:Projectile(sw, IID_TURNIP, startX, startY, 0, 0.5, 1)
{
    setLabel(NEUTRAL);
}

void Turnip::moveProjectile()
{
    moveTo(getX()-6, getY());
    setDirection(getDirection()+20);
}

void Turnip::checkCollision()
{
    if (collideNB())
    {
        setDead();
        getWorld()->getPlayer()->sufferDamage(2.0);
        getWorld()->playSound(SOUND_BLAST);
    }
}

int Turnip::getDamagePoints() const
{
    return TURNIPDAMAGE;
}

///////////////////////////////Torpedo//////////////////////////////////
Torpedo::Torpedo(StudentWorld* sw, double startX, double startY, int d)
:Projectile(sw, IID_TORPEDO, startX, startY, d, 0.5, 1)
{
    setDirection(d);
    // give it a label when fire()
}

void Torpedo::moveProjectile()
{
    if (getLabel() == PLAYER)
        moveTo(getX()+8, getY());
    else
        moveTo(getX()-8, getY());
}

void Torpedo::checkCollision()
{
    double damage = 0.0;
    if (getWorld()->collide(this, damage))
    {
        setDead();
    }
    if (collideNB())
    {
        setDead();
        getWorld()->getPlayer()->sufferDamage(TORPEDODAMAGE);
        getWorld()->playSound(SOUND_BLAST);
    }
}

int Torpedo::getDamagePoints() const
{
    return TORPEDODAMAGE;
}

/////////////////////////////////////////////////////////////////////////
//////////////////////////////// Goodie  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
Goodie::Goodie(StudentWorld* sw, int imageID, double startX, double startY)
:Actor(sw, imageID, startX, startY, 0, 0.5, 1)
{
    setLabel(NEUTRAL);
}

void Goodie::doSomethingDiff()
{
    // Set the goodie to dead if it is off screen.
    if (getY() < 0 || getY() >= VIEW_HEIGHT)
        setDead();
    else
    {
        if (collideNB())
        {
            collisionReaction();
            return;
        }
        if (!dead())
        {
            moveTo(getX()-0.75, getY()-0.75);
            if (collideNB())
            {
                collisionReaction();
                return;
            }
        }
    }
}

void Goodie::collisionReaction()
{
    getWorld()->increaseScore(GOODIESCORE);
    setDead();
    getWorld()->playSound(SOUND_GOODIE);
    bonus();
}

////////////////////////////////  Extralife  ////////////////////////////////
Extralife::Extralife(StudentWorld* sw, double startX, double startY)
: Goodie(sw, IID_LIFE_GOODIE, startX, startY)
{ }

void Extralife::bonus()
{
    getWorld()->incLives();
}


////////////////////////////////   Repair    ////////////////////////////////
Repair::Repair(StudentWorld* sw, double startX, double startY)
: Goodie(sw, IID_REPAIR_GOODIE, startX, startY)
{ }

void Repair::bonus()
{
    double health = getWorld()->getPlayer()->getHitpoints();
    if (health + 10.0 > FULLHITPOINT)
        getWorld()->getPlayer()->setHitpoints(50.0);
    else
        getWorld()->getPlayer()->setHitpoints(getWorld()->getPlayer()->getHitpoints() + 10.0);
}


////////////////////////////////TorpedoGoodie////////////////////////////////
TorpedoGoodie::TorpedoGoodie(StudentWorld* sw, double startX, double startY)
: Goodie(sw, IID_TORPEDO_GOODIE, startX, startY)
{ }

void TorpedoGoodie::bonus()
{
    getWorld()->getPlayer()->addTorpedo(5);
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

void Alien::doSomethingDiff()
{
    if (getX() < 0)
        setDead();
    else
    {
        checkCollision(getDamagePoints(), getScore());
        if (!dead())
        {
            double nbX = getWorld()->getPlayer()->getX();
            double nbY = getWorld()->getPlayer()->getY();
            
            // If need new flight plan
            if (getY() <= 0 || getY() >= VIEW_WIDTH-1 || getLength() <= 0)
                changeDirection();
            
            // If the player is in my sight and can fire
            if (getX() > nbX && getY() <= nbY + 4 && getY() >= nbY - 4)
                fire();
            
            move();
            checkCollision(getDamagePoints(), getScore());
        }
    }
}

void Alien::checkCollision(int damage, int score)
{
    double dp = 0.0;
    if (collideNB())
    {
        getWorld()->getPlayer()->sufferDamage(damage);
        fatalCollision(score);
        dropGoodie();
    }
    else if (getWorld()->collide(this, dp))
    {
        sufferDamage(dp/2);
        if (dead())
        {
            fatalCollision(score);
            dropGoodie();
        }
        else
            getWorld()->playSound(SOUND_BLAST);
    }
}

void Alien::fatalCollision(int score)
{
    getWorld()->increaseScore(score);
    setDead();
    getWorld()->addDestroyed();
    getWorld()->playSound(SOUND_DEATH);
    Explosion* ex = new Explosion(getWorld(), getX(), getY());
    getWorld()->animate(ex);
}

void Alien::changeDirection()
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

void Alien::changeDirDiff()
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

void Alien::moveDiff()
{
    setLength(getLength()-1);
}

bool Alien::alienShip() const
{
    return true;
}

void Alien::sufferDamage(double d)
{
    setHitpoints(getHitpoints()-d);
}

double Alien::getSpeed() const
{
    return m_speed;
}

int Alien::getLength() const
{
    return m_length;
}

int Alien::getTravelDir() const
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

Alien::~Alien()
{
    //std::cerr << "Alien Destructor" << std::endl;
}

//////////////////////////Smallgon////////////////////////////////
Smallgon::Smallgon(StudentWorld* sw, double startX, double startY)
:Alien(sw, IID_SMALLGON, startX, startY)
{
    int n = getWorld()->getLevel();
    double initHP = 5 * (1 + (n-1) * 0.1);
    setHitpoints(initHP);
    setSpeed(SMALLSPEED);
}



void Smallgon::fire()
{
    int chance = randInt(1, (20 / getWorld()->getLevel() + 5));
    if (chance == 1)
    {
        Turnip* t = new Turnip(getWorld(), getX()-14, getY());
        getWorld()->animate(t);
        getWorld()->playSound(SOUND_ALIEN_SHOOT);
    }
}

void Smallgon::dropGoodie()
{
    // Smallgon does not drop goodies.
    return;
}

int Smallgon::getDamagePoints() const
{
    return 5;
}

int Smallgon::getScore() const
{
    return SMALLSCORE;
}

//////////////////////////Smoregon//////////////////////////////////
Smoregon::Smoregon(StudentWorld* sw, double startX, double startY)
:Alien(sw, IID_SMOREGON, startX, startY)
{
    int n = getWorld()->getLevel();
    double initHP = 5 * (1 + (n-1) * 0.1);
    setHitpoints(initHP);
    setSpeed(SMORESPEED);
}

void Smoregon::fire()
{
    int chance = randInt(1, (20 / getWorld()->getLevel() + 5));
    if (chance == 1)
    {
        Turnip* t = new Turnip(getWorld(), getX()-14, getY());
        getWorld()->animate(t);
        getWorld()->playSound(SOUND_ALIEN_SHOOT);
    }
    if (chance == 2)
    {
        setTravelDir(DUELEFT);
        setLength(VIEW_WIDTH);
        setSpeed(5.0);
    }
}

void Smoregon::dropGoodie()
{
    int chance = randInt(1, 3);
    if (chance == 1)
    {
        int goodieType = randInt(1, 2);
        if (goodieType == 1)
        {
            Repair* r = new Repair(getWorld(), getX(), getY());
            getWorld()->animate(r);
        }
        else
        {
            TorpedoGoodie* t = new TorpedoGoodie(getWorld(), getX(), getY());
            getWorld()->animate(t);
        }
    }
}


int Smoregon::getDamagePoints() const
{
    return 5;
}

int Smoregon::getScore() const
{
    return SMORESCORE;
}

//////////////////////////Snagglegon////////////////////////////////
Snagglegon::Snagglegon(StudentWorld* sw, double startX, double startY)
:Alien(sw, IID_SNAGGLEGON, startX, startY)
{
    int n = getWorld()->getLevel();
    double initHP = 10 * (1 + (n-1) * 0.1);
    setHitpoints(initHP);
    setTravelDir(DOWNLEFT);
    setSpeed(SNAGGLESPEED);
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


void Snagglegon::moveDiff()
{
    // Snagglegon does not have a flight length.
    return;
}

void Snagglegon::fire()
{
    int chance = randInt(1, (15 / getWorld()->getLevel() + 10));
    if (chance == 1)
        {
        Torpedo* t = new Torpedo(getWorld(), getX()-14, getY(), 180);
        t->setLabel(NEUTRAL);
        getWorld()->animate(t);
        getWorld()->playSound(SOUND_TORPEDO);
    }
}

void Snagglegon::dropGoodie()
{
    int chance = randInt(1, 6);
    if (chance == 1)
    {
        Extralife* e = new Extralife(getWorld(), getX(), getY());
        getWorld()->animate(e);
    }
}

int Snagglegon::getDamagePoints() const
{
    return 15;
}

int Snagglegon::getScore() const
{
    return SNAGGLESCORE;
}






