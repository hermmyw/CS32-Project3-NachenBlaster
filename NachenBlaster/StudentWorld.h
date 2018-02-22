#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class NachenBlaster;
class Projectile;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual void animate(Actor* obj);
    virtual bool collide();
    NachenBlaster* getPlayer();
    virtual ~StudentWorld();

private:
    int m_destroyed;
    int m_remained;
    int m_total;
    std::vector<Actor*> m_actors;
    NachenBlaster* m_player;
    int m_level;
    int m_nAlien;
    double dist(int x1, int y1, int x2, int y2);
};

#endif // STUDENTWORLD_H_
