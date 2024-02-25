#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <list>

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

  // HELPER FUNCTIONS
  Actor* findEntryAtPos(double x, double y);
  Marble* getMarbleAtPos(double x, double y);
  Player* getPlayer() const;
  bool isPlayerAt(double x, double y);
  void addToActors(Actor* na);
  bool noMoreCrystals();
  void decreaseNumCrystals();
  int getBonus() const;
  void completeLevel();
  void decreaseBonus();

private:
	std::list<Actor*> actors;
	Player* avatar;
	int m_bonus;
	int m_numCrystals;
	bool levelCompleted;

	// HELPER FUNCTIONS
	int loadCurrentLevel(Level& lev);
	std::string getStatus(int score, int lev, int lives, int hp, int ammo, int bonus);
	void setDisplayText();
};

#endif // STUDENTWORLD_H_
