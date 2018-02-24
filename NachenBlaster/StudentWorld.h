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
    void animate(Actor* obj);
    void addDestroyed();
    bool collide(Actor* obj, double& damage);
    NachenBlaster* getPlayer();
    virtual ~StudentWorld();

private:
    // The actor collection:
    std::vector<Actor*> m_actors;
    // The player:
    NachenBlaster* m_player;
    
    int m_destroyed;
    int m_total;
    int m_nAlien;
    
    // Helper functions:
    int getRemained();
    void insertStars();
    void insertAliens();
    void updateDisplayText();
};

#endif // STUDENTWORLD_H_
