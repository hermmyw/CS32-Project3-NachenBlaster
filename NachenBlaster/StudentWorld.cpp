#include "StudentWorld.h"
#include "GameConstants.h"
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
        Star* s = new Star;
        actors.push_back(s);
    }
    NachenBlaster* player = new NachenBlaster;
    destroyed = 0;
    remained = 6 + (4 * m_level);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (int i = 0; i < actors.size(); i++)
    {
        if (!actors.at(i)->die())
        {
            actors.at(i)->doSomething();
            if (player->die())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            else
            {
                if (remained == 0)
                    return GWSTATUS_FINISHED_LEVEL;
                if (actors.at(i)->die())
                {
                    delete actors.at(i);
                    increaseScore(250);
                    return GWSTATUS_CONTINUE_GAME
                }
            }
        }
    }
    string text = "Lives: 3 Health: 100% Score: 24530 Level: 3 Cabbages: 80% Torpedoes: 4";
    setGameStatText(text);
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
}
