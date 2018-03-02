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
	// For every actor, we need to first check if it is alive.
    if (dead())
        return;
    else
    	// Derived classes perform differentiated actions.
        doSomethingDiff();
}

void Actor::sufferDamage(double d)
{
    setDead();
}

bool Actor::alienShip() const
{
	// Default
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
:Actor(sw, IID_NACHENBLASTER, 0, VIEW_HEIGHT/2), m_nCabbage(FULLCABBAGE), m_nTorpedo(INITIALTORPEDO)
{
    setHitpoints(FULLHITPOINT);
    setLabel(PLAYER);
}

void NachenBlaster::doSomethingDiff()
{
	// Get user keyboard input.
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_SPACE:
                if (m_nCabbage >= 5)
                {
                	// Fire 1 cabbage.
                    Cabbage* c = new Cabbage(getWorld(), getX()+12, getY());
                    getWorld()->animate(c);
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                    
                    // Cabbage energy points minus 5.
                    m_nCabbage -= 5;
                }
                break;
            case KEY_PRESS_TAB:
                if (m_nTorpedo > 0)
                {
                	// Fire 1 torpedo.
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
    // Increase cabbage power in any situation.
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
    // A star can't be attacked and can't attack, so it is neutral.
    setLabel(NEUTRAL);
}

void Star::doSomethingDiff()
{
    // The star must move 1 pixel to the left.
    int x = getX()-1;
    moveTo(x, getY());
    // Offscreen
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
	// An explosion can't be attacked and can't attack, so it is neutral.
    setLabel(NEUTRAL);
}

void Explosion::doSomethingDiff()
{
    // Increase size.
    setSize(1.5 * getSize());
    // Keep track of ticks.
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
	// Offscreen.
    if (getX() < 0 || getX() >= VIEW_WIDTH)
        setDead();
    else if (!dead())
    {
        checkCollision();
        if (!dead())
        {
            std::cerr << "Cabbage did not die" << std::endl;
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
	// Player projectile.
    setLabel(PLAYER);
}

void Cabbage::moveProjectile()
{
    moveTo(getX()+8, getY());
    setDirection(getDirection()+20);
}

void Cabbage::checkCollision()
{
	// If it collides with anything, set it to dead.
	// Alien ships will handle damage by themselves.
    if (getWorld()->collideWith(this) != nullptr)
    {
        getWorld()->collideWith(this)->sufferDamage(getDamagePoints());
        setDead();
        if (dead())
            std::cerr << "Cabbage is dead and ready to be destroyed" << std::endl;
        // getWorld()->deleteDead(this);
    }
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
	// If it collides with nachenBlaster.
    if (collideNB())
    {
        setDead();
        getWorld()->getPlayer()->sufferDamage(getDamagePoints());
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
	// If it is a player projectile, it moves right.
    if (getLabel() == PLAYER)
        moveTo(getX()+8, getY());
    // If it is an enemy projectile, it moves left.
    else
        moveTo(getX()-8, getY());
}

void Torpedo::checkCollision()
{
	// Enemy projectile:
	if (getLabel() == ENEMY && collideNB())
    {
        setDead();
        getWorld()->getPlayer()->sufferDamage(getDamagePoints());
        getWorld()->playSound(SOUND_BLAST);
    }
    // Player projectile:
    else if (getLabel() == PLAYER &&
             getWorld()->collideWith(this) != nullptr)
    {
        getWorld()->collideWith(this)->sufferDamage(getDamagePoints());
        setDead();
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
	// Increase the player's score.
    getWorld()->increaseScore(GOODIESCORE);
    setDead();
    getWorld()->playSound(SOUND_GOODIE);
    // Give the player bonus.
    bonus();
}

Goodie::~Goodie()
{
    // std::cerr << "Goodie Destructor" << std::endl;
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
    // If adding hitpoints will cause health to exceed 100%, set it to 100%.
    if (health + 10.0 > FULLHITPOINT)
        getWorld()->getPlayer()->setHitpoints(FULLHITPOINT);
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
	// Random travel directions.
    int randDir = randInt(1, 3);
    setTravelDir(randDir);
    // Random flight lengths.
    int randL = randInt(1, 32);
    setLength(randL);
    // Enemy label
    setLabel(ENEMY);
}

void Alien::doSomethingDiff()
{
	// Offscreen
    if (getX() < 0)
        setDead();
    else
    {
        checkCollision(getDamagePoints(), getScore());  // How much damage will the player suffer or how many scores will the player get
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
            
            // Continue moving
            move();

            // Check again
            checkCollision(getDamagePoints(), getScore());
        }
    }
}

void Alien::checkCollision(int damage, int score)
{
    if (collideNB())
    {
    	// Player suffers damage.
        getWorld()->getPlayer()->sufferDamage(damage);
        // Alien will die.
        fatalCollision(score);
        // Possibly drop goodies.
        dropGoodie();
    }
//    else if (getWorld()->collide(this, dp))
//    {
//        // Since we check collision twice during a tick,
//        // each time the alien will suffer half of the damage caused by player projectiles.
//        sufferDamage(dp/2);
//        if (dead())
//        {
//            fatalCollision(score);
//            dropGoodie();
//        }
//        else
//            getWorld()->playSound(SOUND_BLAST);
//    }
}

void Alien::fatalCollision(int score)
{
    getWorld()->increaseScore(score);
    setDead();
    // Increment destroyed aliens when an alien is killed.
    getWorld()->addDestroyed();
    getWorld()->playSound(SOUND_DEATH);
    // Add an explosion
    Explosion* ex = new Explosion(getWorld(), getX(), getY());
    getWorld()->animate(ex);
}

void Alien::changeDirection()
{
	// Alien reaches top or bottom.
    if (getY() >= VIEW_WIDTH-1)
    {
        setTravelDir(DOWNLEFT);
    }
    else if (getY() <= 0)
    {
        setTravelDir(UPLEFT);
    }
    // Differentiated behaviors.
    changeDirDiff();
}

void Alien::changeDirDiff()
{
	// Default differentiated behaviors.
	// (For smallgons and smoregons)
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
    // Random flight length.
    // (For smallgons and smoregons)
    int randl = randInt(1, 32);
    setLength(randl);
}

void Alien::move()
{
	// Possibly move in three directions.
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
    // Differentiated behaviors.
    moveDiff();
}

void Alien::moveDiff()
{
	// Differentiated behaviors.
	// (For smallgons and smoregons)
    setLength(getLength()-1);
}

bool Alien::alienShip() const
{
    return true;
}

void Alien::sufferDamage(double d)
{
    setHitpoints(getHitpoints()-d);
    if (dead())
    {
        fatalCollision(getScore());
        dropGoodie();
    }
    else
        getWorld()->playSound(SOUND_BLAST);
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
	// Fire a turnip.
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
    return SMALLDAMAGE;
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
	// Fire a turnip.
    int chance = randInt(1, (20 / getWorld()->getLevel() + 5));
    if (chance == 1)
    {
        Turnip* t = new Turnip(getWorld(), getX()-14, getY());
        getWorld()->animate(t);
        getWorld()->playSound(SOUND_ALIEN_SHOOT);
    }
    // Or accelerate.
    else if (chance == 2)
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
    	// Drop a repair goodie.
        int goodieType = randInt(1, 2);
        if (goodieType == 1)
        {
            Repair* r = new Repair(getWorld(), getX(), getY());
            getWorld()->animate(r);
        }
        // ...Or a torpedo goodie.
        else
        {
            TorpedoGoodie* t = new TorpedoGoodie(getWorld(), getX(), getY());
            getWorld()->animate(t);
        }
    }
}


int Smoregon::getDamagePoints() const
{
    return SMOREDAMAGE;
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
	// Snagglegon only moves in two directions.
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
	// Fire a torpedo.
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
	// Drop Extralife goodie.
    int chance = randInt(1, 6);
    if (chance == 1)
    {
        Extralife* e = new Extralife(getWorld(), getX(), getY());
        getWorld()->animate(e);
    }
}

int Snagglegon::getDamagePoints() const
{
    return SNAGGLEDAMAGE;
}

int Snagglegon::getScore() const
{
    return SNAGGLESCORE;
}






