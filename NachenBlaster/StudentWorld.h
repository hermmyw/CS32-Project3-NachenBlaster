#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class NachenBlaster;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
    int destroyed;
    int remained;
    int total;
    std::vector<Actor*> actors;
    NachenBlaster* player;
    int m_level;
};

#endif // STUDENTWORLD_H_
