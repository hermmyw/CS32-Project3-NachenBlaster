#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>
#include <cmath>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int UPLEFT = 1;
const int DOWNLEFT = 2;
const int DUELEFT = 3;

const int PLAYER = 1;
const int ENEMY = 2;
const int NEUTRAL = 3;

class StudentWorld;
class Actor : public GraphObject
{
public:
    // Constructor
    Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    
    // Virtual functions
    virtual void doSomething();    // check alive
    virtual void sufferDamage(int d);
    virtual bool isAlien();
    virtual int getDamagePoints();
    
    // Accessor
    double getHitpoints();
    int getLabel();
    StudentWorld* getWorld();
    
    // Mutator function
    void setHitpoints(double hp);
    void setDead();
    void setLabel(int n);
    
    // Check Status
    bool die();
    bool offScreen(int x, int y);
    bool collideNB();
    
    // Destructor
    virtual ~Actor();
    
private:
    int m_label;
    double dist(int x1, int y1, int x2, int y2);
    double m_hitpoints;
    virtual void doSomethingDiff() = 0;
    StudentWorld* m_world;
};

class NachenBlaster : public Actor
{
public:
    NachenBlaster(StudentWorld* sw);
    virtual void sufferDamage(int d);
    int getCabbage();
    int getTorpedo();
    void addTorpedo(int n);
    virtual ~NachenBlaster();
private:
    virtual void doSomethingDiff();
    int m_nCabbage;
    int m_nTorpedo;
};





class Star : public Actor
{
public:
    Star(StudentWorld*sw, int x, int y);
    virtual ~Star();
private:
    virtual void doSomethingDiff();
};





class Alien : public Actor
{
public:
    Alien(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.5, int depth = 1);
    virtual bool isAlien();
    virtual void sufferDamage(int d);
    virtual void changeDir();
    virtual void fire();
    virtual void move();
    virtual void checkCollision() = 0;
    virtual void dropGoodie();   // for smoregon and snagglegon
    virtual void fatalCollision(int score);
    
    double getSpeed();
    int getLength();
    int getTravelDir();
    
    void setSpeed(double s);
    void setLength(int l);
    void setTravelDir(int tr);
    
    virtual ~Alien();
private:
    virtual void doSomethingDiff();
    virtual void changeDirDiff() = 0;
    virtual void fireDiff() = 0;
    virtual void moveDiff() = 0;
    double m_speed;
    int m_length;
    int m_travelDir;
};




class Smallgon : public Alien
{
public:
    Smallgon(StudentWorld* sw, double startX, double startY);
    virtual void changeDirDiff();
    virtual void fireDiff();
    virtual void moveDiff();
    virtual void dropGoodie();
    virtual void checkCollision();
};



class Smoregon : public Alien
{
public:
    Smoregon(StudentWorld* sw, double startX, double startY);
    virtual void changeDirDiff();
    virtual void fireDiff();
    virtual void moveDiff();
    virtual void dropGoodie();
    virtual void checkCollision();
};



class Snagglegon : public Alien
{
public:
    Snagglegon(StudentWorld* sw, double startX, double startY);
    virtual void changeDirDiff();
    virtual void fireDiff();
    virtual void moveDiff();
    virtual void dropGoodie();
    virtual void checkCollision();
};







class Projectile : public Actor
{
public:
    Projectile(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    virtual void checkCollision() = 0;
    virtual int getDamagePoints() = 0;
    virtual void sufferDamage(int d);
    virtual void doSomethingDiff();
    virtual ~Projectile();
private:
    
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* sw, int x, int y);
    virtual void checkCollision();
    virtual int getDamagePoints();
private:
    virtual void doSomethingDiff();
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* sw, int x, int y);
    virtual void checkCollision();
    virtual int getDamagePoints();

private:
    virtual void doSomethingDiff();
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* sw, int x, int y, int d);
    virtual void checkCollision();
    virtual int getDamagePoints();

private:
    virtual void doSomethingDiff();
};







class Goodie : public Actor
{
public:
    Goodie(StudentWorld* sw, int imageID, int x, int y);
    
private:
    virtual void doSomethingDiff();
    virtual void collisionReaction();
    virtual void bonus() = 0;
};

class Extralife : public Goodie
{
public:
    Extralife(StudentWorld* sw, int startX, int startY);
private:
    virtual void bonus();
};

class Repair : public Goodie
{
public:
    Repair(StudentWorld* sw, int startX, int startY);
private:
    virtual void bonus();
};

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(StudentWorld* sw, int startX, int startY);
private:
    virtual void bonus();
};

class Explosion : public Actor
{
public:
    Explosion(StudentWorld* sw, int x, int y);
    int getTickCount();
private:
    virtual void doSomethingDiff();
    int m_tick;
};


#endif // ACTOR_H_
