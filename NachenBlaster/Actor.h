#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>
#include <cmath>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
const int UPLEFT = 1;
const int DOWNLEFT = 2;
const int DUELEFT = 3;

class StudentWorld;
class Actor : public GraphObject
{
public:
    // Constructor
    Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    
    // Virtual functions
    virtual void doSomething();
    virtual void sufferDamage(int d);
    virtual bool collide(Actor* obj);
    virtual bool isAlien();
    virtual bool collidableWithNB() = 0;
    virtual bool collidableWithAlien() = 0;
    
    // Mutator function
    void setHitpoints(double hp);
    void setDead();
    
    // Check Status
    bool die();
    bool offScreen(int x, int y);

    // Accessor
    double getHitpoints();
    StudentWorld* getWorld();
    
    // Destructor
    virtual ~Actor();
    
private:
    double dist(int x1, int y1, int x2, int y2);
    double m_hitpoints;
    StudentWorld* m_world;
};

class NachenBlaster : public Actor
{
public:
    NachenBlaster(StudentWorld* sw);
    virtual void doSomething();
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
    int getCabbage();
    int getTorpedo();
    virtual ~NachenBlaster();
private:
    int m_nCabbage;
    int m_nTorpedo;
};
class Star : public Actor
{
public:
    Star(StudentWorld*sw, int x, int y, double r);
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
    virtual void doSomething();
    virtual ~Star();
};
class Alien : public Actor
{
public:
    Alien(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.5, int depth = 1);
    virtual bool isAlien();
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
    virtual void doSomething();
    virtual void changeDir();
    virtual void fire();
    virtual void move();
    virtual void collisionReaction();
    virtual void dropGoodie() = 0;
    double getSpeed();
    int getLength();
    int getTravelDir();
    void setSpeed(double s);
    void setLength(int l);
    void setTravelDir(int tr);
private:
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
    virtual void doSomething();
    virtual void changeDirDiff();
    virtual void fireDiff();
    virtual void moveDiff();
    virtual void dropGoodie();
};
class Smoregon : public Alien
{
public:
    Smoregon(StudentWorld* sw, double startX, double startY);
    virtual void doSomething();
    virtual void changeDirDiff();
    virtual void fireDiff();
    virtual void moveDiff();
    virtual void dropGoodie();
};
class Snagglegon : public Alien
{
public:
    Snagglegon(StudentWorld* sw, double startX, double startY, int travelDir);
    virtual void doSomething();
    virtual void changeDirDiff();
    virtual void fireDiff();
    virtual void moveDiff();
    virtual void dropGoodie();
};
class Projectile : public Actor
{
public:
    Projectile(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    virtual bool collidableWithNB() = 0;
    virtual bool collidableWithAlien() = 0;
    virtual void doSomething() = 0;
    virtual ~Projectile();
private:
    StudentWorld* m_world;
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* sw, int x, int y);
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
    virtual void doSomething();
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* sw, int x, int y);
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
    virtual void doSomething();
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* sw, int x, int y, int d);
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
    virtual void doSomething();
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* sw, int x, int y);
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
};

class Explosion : public Actor
{
public:
    Explosion(StudentWorld* sw, int x, int y);
    virtual bool collidableWithNB();
    virtual bool collidableWithAlien();
    virtual void doSomething();
    int getTickCount();
private:
    int m_tick;
};


#endif // ACTOR_H_
