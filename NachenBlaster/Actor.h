#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <iostream>
#include <cmath>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor : public GraphObject
{
public:
    // Constructor
    Actor(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    
    // Virtual functions
    virtual void doSomething() = 0;
    virtual void sufferDamage();
    
    
    bool collide(int x1, int y1, int r1, int x2, int y2, int r2);
    void setHitpoints(int hp);
    void setDead();
    bool die();
    bool offScreen(int x, int y);
    
    // Accessor
    int getHitpoints();
    StudentWorld* getWorld();
    
    // Destructor
    virtual ~Actor();
    
private:
    double dist(int x1, int y1, int x2, int y2);
    int m_hitpoints;
    StudentWorld* m_world;
};

class NachenBlaster : public Actor
{
public:
    NachenBlaster(StudentWorld* sw);
    virtual void doSomething();
    virtual void sufferDamage();
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
    virtual void doSomething();
    virtual ~Star();
};
class Alien : public Actor
{

};
class Smallgon : public Alien
{
    
};
class Smoregon : public Alien
{
    
};
class Snagglegon : public Alien
{
    
};
class Projectile : public Actor
{
public:
    Projectile(StudentWorld* sw, int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    virtual void doSomething();
    // virtual void animate();
    virtual ~Projectile();
private:
    StudentWorld* m_world;
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* sw, int x, int y, int d);
    virtual void doSomething();
};
class Goodie : public Actor
{
    
};
class Explosion : public Actor
{
    
};
#endif // ACTOR_H_
