#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp
class Actor;
class NachenBlaster;
class Star;

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{ }

int StudentWorld::init()
{
    m_level = getLevel();
    for (int i = 0; i < 30; i++)
    {
        int randomX = randInt(0, 255);
        int randomY = randInt(0, 255);
        double randomSize = randInt(5, 50) / 100.0;
        Star* s = new Star(this, randomX, randomY, randomSize);
        m_actors.push_back(s);
    }
    m_player = new NachenBlaster(this);
    destroyed = 0;
    remained = 6 + (4 * m_level);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (!m_actors.at(i)->die())
            m_actors.at(i)->doSomething();
        else
        {
            Actor* temp = m_actors.at(i);
            m_actors.erase(m_actors.begin()+i);
            delete temp;
            i--;
        }
        
    }
    if (m_player->die())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    else
    {
        if (remained == 0)
            return GWSTATUS_FINISHED_LEVEL;
        m_player->doSomething();
    }
    
    // insert new stars
    int r = randInt(1, 15);
    if (r == 1)
    {
        int randomY = randInt(0, 255);
        int r = randInt(5, 50);
        double randomSize = r / 100.0;
        Star* s = new Star(this, 255, randomY, randomSize);
        m_actors.push_back(s);
    }
    
    // Update game stats text
    ostringstream oss;
    oss << "Lives: " << getLives();
    oss << "  Health: " << m_player->getHitpoints()*100/50 << "%";
    oss << "  Score: " << getScore();
    oss << "  Level: " << getLevel();
    oss << "  Cabbages: " << m_player->getCabbage()*100/30 << "%";
    oss << "  Torpedoes: " << m_player->getTorpedo();
    string text = oss.str();
    setGameStatText(text);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // stars, aliens, explosions, projectiles...
    while (!m_actors.empty())
    {
        Actor* temp = m_actors.back();
        m_actors.pop_back();
        delete temp;
    }
    delete m_player;
}

void StudentWorld::animate(Projectile* proj)
{
    proj->doSomething();
    m_actors.push_back(proj);
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}
