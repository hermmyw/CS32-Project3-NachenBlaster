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
    Actor(int imageID, double startX, double startY, int dir = 0, double size = 1.0, int depth = 0);
    virtual void doSomething() = 0;
    bool collide(int x1, int y1, int r1, int x2, int y2, int r2);
    virtual void collisionReaction();
    bool die();
    void setHitpoints(int hp);
    bool offScreen();
    StudentWorld* getWorld();
    virtual ~Actor();
private:
    double dist(int x1, int y1, int x2, int y2);
    int m_hitpoints;
    StudentWorld* m_world;
};

class NachenBlaster : public Actor
{
public:
    NachenBlaster();
    virtual void doSomething();
    virtual ~NachenBlaster();
private:
    int m_nCabbage;
    int m_nTorpedo;
};
class Star : public Actor
{
public:
    Star(int x, int y, int r);
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
    
};
class Cabbage : public Projectile
{
    
};
class Turnip : public Projectile
{
    
};
class Torpedo : public Projectile
{
    
};
class Goodie : public Actor
{
    
};
class Explosion : public Actor
{
    
};
#endif // ACTOR_H_
