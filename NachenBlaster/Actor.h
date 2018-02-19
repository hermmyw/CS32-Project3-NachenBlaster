#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor
{
public:
    Actor();
    virtual ~Actor();
private:
};

class NachenBlaster : Actor
{
public:
    NachenBlaster();
    virtual ~NachenBlaster();
};

class Star : Actor
{
public:
    Star();
    virtual ~Star();
};
#endif // ACTOR_H_
