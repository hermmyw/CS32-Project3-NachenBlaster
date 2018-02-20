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
{
}

int StudentWorld::init()
{
    m_level = getLevel();
    for (int i = 0; i < 30; i++)
    {
        int randomX = randInt(0, 255);
        int randomY = randInt(0, 255);
        double randomSize = randInt(5, 50) / 100.0;
        Star* s = new Star(randomX, randomY, randomSize);
        m_actors.push_back(s);
    }
    //NachenBlaster* player = new NachenBlaster;
    //destroyed = 0;
    //remained = 6 + (4 * m_level);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (int i = 0; i < m_actors.size(); i++)
    {
        m_actors.at(i)->doSomething();
        if (!m_actors.at(i)->die())
        {
            m_actors.at(i)->doSomething();
            if (player->die())
            {
                decLives();
                return GWSTATUS_CONTINUE_GAME;
            }
            else
            {
                if (remained == 0)
                    return GWSTATUS_FINISHED_LEVEL;
                if (m_actors.at(i)->die())
                {
                    delete m_actors.at(i);
                    increaseScore(250);
                    return GWSTATUS_CONTINUE_GAME;
                }
            }
        }
    }
    string text = "Lives: 3 Health: 100% Score: 24530 Level: 3 Cabbages: 80% Torpedoes: 4";
    setGameStatText(text);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}
