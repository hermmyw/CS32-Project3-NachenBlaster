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
    // Create 30 stars and push them into the actor collection.
    for (int i = 0; i < 30; i++)
    {
        double randomX = randInt(0, VIEW_WIDTH-1);
        double randomY = randInt(0, VIEW_HEIGHT-1);
        Star* s = new Star(this, randomX, randomY);
        m_actors.push_back(s);
    }
    
    // Create a nachenblaster.
    m_player = new NachenBlaster(this);
    
    // Initialize data members.
    m_nAlien = 0;
    m_destroyed = 0;
    m_total = 6 + (4 * getLevel());
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (!m_actors.at(i)->dead())
        {
            m_actors.at(i)->doSomething();
            // If an actor's action causes the player to die, end the function.
            if (m_player->dead())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            // Otherwise, check if the level is finished.
            else
            {
                // The player completed the level.
                if (getRemained() <= 0)
                {
                    playSound(SOUND_FINISHED_LEVEL);
                    return GWSTATUS_FINISHED_LEVEL;
                }
            }
        }
        else
        // Delete dead objects.
        {
            Actor* temp = m_actors.at(i);
            m_actors.erase(m_actors.begin()+i);
            if (temp->alienShip())
                m_nAlien--;
            delete temp;
            i--;
        }
    }
    
    // Let the player do something.
    m_player->doSomething();

    // Update game status.
    updateDisplayText();   // update the game stats at screen top.
    
    // Insert new stars and new alien ships.
    insertStars();
    insertAliens();
    
    // Continue if the player is alive and the level is not completed.
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
            double x1 = m_actors.at(i)->getX();
            double y1 = m_actors.at(i)->getY();
            double r1 = m_actors.at(i)->getRadius();
            int label1 = m_actors.at(i)->getLabel();
            double x2 = obj->getX();
            double y2 = obj->getY();
            double r2 = obj->getRadius();
            int label2 = obj->getLabel();
            if (label1 != NEUTRAL && label2 != NEUTRAL && label1 != label2 && dist(x1, y1, x2, y2) < 0.75 * (r1 + r2))
            // label1 = player proj, label2 = enemy or enemy proj
            // label1 = enemy or enemy proj, label2 = player proj
            // let obj be enemy...and check for collision w/ player proj
            {
                
                if (m_actors.at(i)->getLabel() == PLAYER)
                {
                    damage = m_actors.at(i)->getDamagePoints();
                    m_actors.at(i)->setDead();
                }
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::addDestroyed()
{
    m_destroyed++;
}

int StudentWorld::getRemained()
{
    return m_total - m_destroyed;
}

NachenBlaster* StudentWorld::getPlayer()
{
    return m_player;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}


//////////////////////////////////////////////////////////////////////
/////////////////////  Private Helper Functions  /////////////////////
//////////////////////////////////////////////////////////////////////
void StudentWorld::insertStars()
{
    // Insert new stars in 1/15 chance.
    int chanceStar = randInt(1, 15);
    if (chanceStar == 1)
    {
        double randomY = randInt(0, VIEW_HEIGHT-1);
        Star* s = new Star(this, VIEW_HEIGHT-1, randomY);
        m_actors.push_back(s);
    }
}

void StudentWorld::insertAliens()
{
    // Insert new ships.
    double maximum = 4 + (0.5 * getLevel());
    double min = (maximum < getRemained()) ? maximum : getRemained();
    if (m_nAlien < min)
    {
        Alien* a;
        double randomY = randInt(0, VIEW_HEIGHT-1);
        
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
        
        // Add the new alien ship to the actor collection.
        // Increment the number of alien ships.
        m_actors.push_back(a);
        m_nAlien++;
    }
}

void StudentWorld::updateDisplayText()
{
    ostringstream oss;
    oss << "Lives: " << getLives();
    oss << "  Health: " << m_player->getHitpoints()*100/50 << "%";
    oss << "  Score: " << getScore();
    oss << "  Level: " << getLevel();
    oss << "  Cabbages: " << m_player->getCabbage()*100/30 << "%";
    oss << "  Torpedoes: " << m_player->getTorpedo();
    string text = oss.str();
    setGameStatText(text);
}


