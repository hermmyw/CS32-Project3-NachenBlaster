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
    // create 30 stars
    for (int i = 0; i < 30; i++)
    {
        int randomX = randInt(0, VIEW_WIDTH-1);
        int randomY = randInt(0, VIEW_HEIGHT-1);
        double randomSize = randInt(5, 50) / 100.0;
        Star* s = new Star(this, randomX, randomY, randomSize);
        m_actors.push_back(s);
    }
    
    // create a nachenblaster
    m_player = new NachenBlaster(this);
    
    // initialize data members
    m_nAlien = 0;
    m_destroyed = 0;
    m_total = 6 + (4 * getLevel());
    m_remained = m_total - m_destroyed;
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
        // free dead objects
        {
            Actor* temp = m_actors.at(i);
            m_actors.erase(m_actors.begin()+i);
            if (temp->isAlien())
                m_nAlien--;
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
        if (m_remained == 0)
            return GWSTATUS_FINISHED_LEVEL;
        m_player->doSomething();
    }
    
    // insert new stars
    int r = randInt(1, 15);
    if (r == 1)
    {
        int randomY = randInt(0, VIEW_HEIGHT-1);
        int r = randInt(5, 50);
        double randomSize = r / 100.0;
        Star* s = new Star(this, 255, randomY, randomSize);
        m_actors.push_back(s);
    }
    
    // insert new ships
    int maximum = 4 + (0.5 * getLevel());
    int min = (maximum < m_remained) ? maximum : m_remained;
    if (m_nAlien < min)
    {
        Alien* a;
        int randomY = randInt(0, VIEW_HEIGHT-1);
        
        int S1 = 60;
        int S2 = 20 + getLevel() * 5;
        int S3 = 5 + getLevel() * 10;
        int S = S1 + S2 + S3;
        int randShip = randInt(1, S);
        if (randShip >= 1 && randShip <= S1)
        {
            a = new Smallgon(this, VIEW_WIDTH-1, randomY);
        }
        else if (randShip <= S1 + S2)
        {
            a = new Smoregon(this, VIEW_WIDTH-1, randomY);
        }
        else
        {
            a = new Snagglegon(this, VIEW_WIDTH-1, randomY, 0);
        }
        m_actors.push_back(a);
        m_nAlien++;
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
    m_actors.clear();
}

void StudentWorld::animate(Actor* obj)
{
    obj->doSomething();
    m_actors.push_back(obj);
}

bool StudentWorld::collide()
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        int x1 = m_actors.at(i)->getX();
        int y1 = m_actors.at(i)->getY();
        int r1 = m_actors.at(i)->getRadius();
        int xp = m_player->getX();
        int yp = m_player->getY();
        int rp = m_player->getRadius();
        // if a alien/alien's projectile collides with a nachenblaster
        // if a alien collides with a nachenblaster's projectile
        
        // alien-NB
        // alienProj-NB
        //if (m_actors.at(i)->collidableWithNB())
        if (m_actors.at(i)->isAlien())
        {
            if (dist(x1, y1, xp, yp) < 0.75 * (r1 + rp))
            {
                m_actors.at(i)->collisionReaction();
            }
        }
        
        // NBproj-alien
        // NBproj!=alienProj
        if (m_actors.at(i)->collidableWithAlien())
        {
            for (int j = i+1; j < m_actors.size(); j++)
            {
                int x2 = m_actors.at(j)->getX();
                int y2 = m_actors.at(j)->getY();
                int r2 = m_actors.at(j)->getRadius();
                if (dist(x1, y1, x2, y2) < 0.75 * (r1 + r2))
                    return true;
            }
        }
    }
    return false;
}

NachenBlaster* StudentWorld::getPlayer()
{
    return m_player;
}

double StudentWorld::dist(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}
