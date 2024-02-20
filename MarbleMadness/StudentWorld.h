#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();


private:
	std::vector<Actor*> actors;
	Player* avatar;
	int m_numActors;
	int m_bonus;
	void getStatus(int score, int lev, int lives, int hp, int ammo, int bonus);
	void setDisplayText();
};

#endif // STUDENTWORLD_H_
