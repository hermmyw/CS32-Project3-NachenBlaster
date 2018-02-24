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
    // create 30 stars
    m_level = getLevel();
    for (int i = 0; i < 30; i++)
    {
        int randomX = randInt(0, VIEW_WIDTH-1);
        int randomY = randInt(0, VIEW_HEIGHT-1);
        Star* s = new Star(this, randomX, randomY);
        m_actors.push_back(s);
    }
    
    // create a nachenblaster
    m_player = new NachenBlaster(this);
    
    // initialize data members
    m_nAlien = 0;
    m_destroyed = 0;
    m_total = 6 + (4 * getLevel());
    m_remained = m_total;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::addDestroyed()
{
    m_destroyed++;
    m_remained--;
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
        Star* s = new Star(this, VIEW_HEIGHT-1, randomY);
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
            a = new Smallgon(this, VIEW_WIDTH-1, randomY);
        else if (randShip <= S1 + S2)
            a = new Smoregon(this, VIEW_WIDTH-1, randomY);
        else
            a = new Snagglegon(this, VIEW_WIDTH-1, randomY);
        
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
    if (!m_actors.empty())
    {
        m_actors.push_back(m_player);
        vector<Actor*>::iterator p = m_actors.begin();
        while (p != m_actors.end())
        {
            delete *p;
            p = m_actors.erase(p);
        }
    }
}

void StudentWorld::animate(Actor* obj)
{
    obj->doSomething();
    m_actors.push_back(obj);
}

bool StudentWorld::collide(Actor* obj, double& damage)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (obj != m_actors.at(i))
        {
            int x1 = m_actors.at(i)->getX();
            int y1 = m_actors.at(i)->getY();
            int r1 = m_actors.at(i)->getRadius();
            int label1 = m_actors.at(i)->getLabel();
            int x2 = obj->getX();
            int y2 = obj->getY();
            int r2 = obj->getRadius();
            int label2 = obj->getLabel();
            if (label1 != NEUTRAL && label2 != NEUTRAL && label1 != label2 && dist(x1, y1, x2, y2) < 0.75 * (r1 + r2))
            // label1 = player proj, label2 = enemy or enemy proj
            // label1 = enemy or enemy proj, label2 = player proj
            // let obj be enemy...and check for collision w/ player proj
            {
//                if (label2 == ENEMY)  // obj is an alien, m_actors[i] is a player proj
//                {
//                    obj->sufferDamage(2);
//                    m_actors.at(i)->setDead();
//                }
//                else if (label1 == ENEMY)  // obj is a player proj, m_actors[i] is an alien
//                    m_actors.at(i)->sufferDamage(2);
                if (m_actors.at(i)->getLabel() == PLAYER)
<<<<<<< HEAD
                {
                    damage = m_actors.at(i)->getDamagePoints();
                    m_actors.at(i)->setDead();
                }
=======
                    damage = m_actors.at(i)->getDamagePoints();
>>>>>>> eafea760b0fa5e21a960de142b591b2f91ddc529
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
