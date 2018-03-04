#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>
#include <cmath>

// Constants
const int UPLEFT = 1;
const int DOWNLEFT = 2;
const int DUELEFT = 3;

const int PLAYER = 1;
const int ENEMY = 2;
const int NEUTRAL = 3;

const double FULLHITPOINT = 50.0;
const int FULLCABBAGE = 30;
const int INITIALTORPEDO = 30;

const int CABBAGEDAMAGE = 2;
const int TURNIPDAMAGE = 2;
const int TORPEDODAMAGE = 8;

const int SMALLSCORE = 250;
const int SMORESCORE = 250;
const int SNAGGLESCORE = 1000;
const int GOODIESCORE = 100;

const double SMALLSPEED = 2.0;
const double SMORESPEED = 2.0;
const double SNAGGLESPEED = 1.75;

const int SMALLDAMAGE = 5;
const int SMOREDAMAGE = 5;
const int SNAGGLEDAMAGE = 15;
// Helper function.
double dist(double x1, double y1, double x2, double y2);

class StudentWorld;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////   Actor    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class Actor : public GraphObject
{
public:
    // Constructor
    Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    // This function only checks if an actor is alive.
    void doSomething();    
    // Derived classes perform differentiated actions.
    virtual void doSomethingDiff() = 0;
    virtual void sufferDamage(double d);
    virtual bool alienShip() const;
    virtual int getDamagePoints() const;
    double getHitpoints() const;
    int getLabel() const;
    void setHitpoints(double hp);
    void setDead();
    void setLabel(int n);
    bool dead() const;
    bool offScreen(double x, double y) const;
    bool collideNB() const;
    StudentWorld* getWorld() const;
    
    // Destructor
    virtual ~Actor();
    
private:
    // Label actors as player and enemy.
    int m_label;
    double m_hitpoints;
    StudentWorld* m_world;
};


/////////////////////////////////////////////////////////////////////////
////////////////////////////NachenBlaster////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class NachenBlaster : public Actor
{
public:
    NachenBlaster(StudentWorld* sw);
    virtual void sufferDamage(double d);
    int getCabbage() const;
    int getTorpedo() const;
    void addTorpedo(int n);
    virtual ~NachenBlaster();
private:
    virtual void doSomethingDiff();
    int m_nCabbage;
    int m_nTorpedo;
};




/////////////////////////////////////////////////////////////////////////
/////////////////////////////    Star    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class Star : public Actor
{
public:
    Star(StudentWorld*sw, double startX, double startY);
    virtual ~Star();
private:
    virtual void doSomethingDiff();
};


/////////////////////////////////////////////////////////////////////////
///////////////////////////// Explosion  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class Explosion : public Actor
{
public:
    Explosion(StudentWorld* sw, double startX, double startY);
private:
    virtual void doSomethingDiff();
    int m_tick;
};


/////////////////////////////////////////////////////////////////////////
///////////////////////////// Projectile ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class Projectile : public Actor
{
public:
    Projectile(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    virtual void checkCollision() = 0;
    virtual int getDamagePoints() const = 0;
    virtual void sufferDamage(double d);
    virtual void doSomethingDiff();
    virtual ~Projectile();
private:
    virtual void moveProjectile() = 0;
};

///////////////////////////////Cabbage//////////////////////////////////
class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* sw, double startX, double startY);
    virtual void checkCollision();
    virtual int getDamagePoints() const;
    
private:
    virtual void moveProjectile();
};

//////////////////////////////Turnip///////////////////////////////////
class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* sw, double startX, double startY);
    virtual void checkCollision();
    virtual int getDamagePoints() const;
    
private:
    virtual void moveProjectile();
};

///////////////////////////////Torpedo//////////////////////////////////
class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* sw, double startX, double startY, int d);
    virtual void checkCollision();
    virtual int getDamagePoints() const;
    
private:
    virtual void moveProjectile();
};




/////////////////////////////////////////////////////////////////////////
//////////////////////////////// Goodie  ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class Goodie : public Actor
{
public:
    Goodie(StudentWorld* sw, int imageID, double startX, double startY);
    virtual ~Goodie();
private:
    virtual void doSomethingDiff();
    void collisionReaction();
    virtual void bonus() = 0;
};

///////////////////////////////  Extralife  /////////////////////////////
class Extralife : public Goodie
{
public:
    Extralife(StudentWorld* sw, double startX, double startY);
private:
    virtual void bonus();
};

///////////////////////////////   Repair    /////////////////////////////
class Repair : public Goodie
{
public:
    Repair(StudentWorld* sw, double startX, double startY);
private:
    virtual void bonus();
};

///////////////////////////////TorpedoGoodie/////////////////////////////
class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(StudentWorld* sw, double startX, double startY);
private:
    virtual void bonus();
};



/////////////////////////////////////////////////////////////////////////
/////////////////////////////   Alien    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class Alien : public Actor
{
public:
    Alien(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.5, int depth = 1);
    virtual bool alienShip() const;
    virtual void sufferDamage(double d);
    double getSpeed() const;
    int getLength() const;
    void setSpeed(double s);
    void setLength(int l);
    void setTravelDir(int tr);
    virtual ~Alien();
private:
    virtual void doSomethingDiff();
    void checkCollision(int damage, int score);
    virtual void fatalCollision(int score);
    void changeDirection();
    virtual void changeDirDiff();   // Default version of a differentiated function
    void move();
    virtual void moveDiff();   // Default version in base of a differentiated function move
    virtual void dropGoodie() = 0;   // For smoregon and snagglegon
    virtual void fire() = 0;
    virtual int getDamagePoints() const = 0;
    virtual int getScore() const = 0;
    int getTravelDir() const;
    double m_speed;
    int m_length;
    int m_travelDir;
};

//////////////////////////Smallgon////////////////////////////////
class Smallgon : public Alien
{
public:
    Smallgon(StudentWorld* sw, double startX, double startY);
private:
    virtual void fire();
    virtual void dropGoodie();
    virtual int getDamagePoints() const;
    virtual int getScore() const;
};


//////////////////////////Smoregon//////////////////////////////////
class Smoregon : public Alien
{
public:
    Smoregon(StudentWorld* sw, double startX, double startY);
private:
    virtual void fire();
    virtual void dropGoodie();
    virtual int getDamagePoints() const;
    virtual int getScore() const;
};


//////////////////////////Snagglegon////////////////////////////////
class Snagglegon : public Alien
{
public:
    Snagglegon(StudentWorld* sw, double startX, double startY);
private:
    virtual void changeDirDiff();  // Snagglegon changes direction differently
    virtual void moveDiff();       // Snagglegon moves differently
    virtual void fire();
    virtual void dropGoodie();
    virtual int getDamagePoints() const;
    virtual int getScore() const;
};


#endif // ACTOR_H_
