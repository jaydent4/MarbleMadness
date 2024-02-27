#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

// CONSTANTS
static const int RIGHT = 0;
static const int LEFT = 180;
static const int UP = 90;
static const int DOWN = 270;

class Actor;
class Marble;
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
	Actor* getThiefBotAtPos(double x, double y);
	Marble* getMarbleAtPos(double x, double y);
	void addToActors(Actor* na);
	Player* getPlayer() const;
	void decreaseNumCrystals();
	bool noMoreCrystals();
	int getBonus() const;
	bool isPlayerAt(double x, double y);
	void completeLevel();
	void decreaseBonus();
	bool clearShotToPlayerExists(double ix, double iy, int dir);

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
	bool isEntryAlive(Actor* entry);
	void removeDeadEntries();
};

#endif // STUDENTWORLD_H_