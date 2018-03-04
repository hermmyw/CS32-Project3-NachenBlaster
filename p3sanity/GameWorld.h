#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include "GameConstants.h"
#include <string>

const int START_PLAYER_LIVES = 3;

class GameController;

class GameWorld
{
public:

	GameWorld(std::string assetDir)
	 : m_lives(START_PLAYER_LIVES), m_score(0), m_level(1),
	   m_controller(nullptr), m_assetDir(assetDir)
	{
	}

	virtual ~GameWorld()
	{
	}
	
	virtual int init() = 0;
	virtual int move() = 0;
	virtual void cleanUp() = 0;

	void setGameStatText(std::string /* text */)
	{
	}

      // ***** Added for testing *****
private:
	int m_key = -1;
public:
	void setKey(int key)
	{
		m_key = key;
	}

	bool getKey(int& value)
	{
		if (m_key == -1)
			return false;
		value = m_key;
		m_key = -1;
		return true;
	}

	void playSound(int /* soundID */)
	{
	}

	unsigned int getLevel() const
	{
		return m_level;
	}

	unsigned int getLives() const
	{
		return m_lives;
	}
	
	void decLives()
	{
		m_lives--;
	}
	
	void incLives()
    {
		m_lives++;
	}

	unsigned int getScore() const
	{
		return m_score;
	}
	
	void increaseScore(unsigned int howMuch)
	{
		m_score += howMuch;
	}
	
	  // The following should be used by only the framework, not the student

	bool isGameOver() const
	{
		return m_lives == 0;
	}

	void advanceToNextLevel() 
	{
		++m_level;
	}
   
	void setController(GameController* controller)
	{
		m_controller = controller;
	}

	std::string assetDirectory() const
	{
		return m_assetDir;
	}
	
private:
	unsigned int	m_lives;
	unsigned int	m_score;
	unsigned int	m_level;
	GameController* m_controller;
	std::string		m_assetDir;
};

#endif // GAMEWORLD_H_
