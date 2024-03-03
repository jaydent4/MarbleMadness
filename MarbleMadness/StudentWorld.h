#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Marble;
class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();

	// GAME FUNCTIONALITY FUNCTIONS
	virtual int init();
	virtual int move();
	virtual void cleanUp();

	// STATUS FUNCTIONS
	bool noMoreCrystals();
	bool isPlayerAt(double x, double y);
	bool clearShotToPlayerExists(double ix, double iy, int dir);

	// ACCESSOR FUNCTIONS
	Actor* findEntryAtPos(double x, double y);
	Actor* getThiefBotAtPos(double x, double y);
	Actor* getMarbleAtPos(double x, double y);
	Player* getPlayer() const;
	bool posStopsPeas(double x, double y, Actor*& entry);
	bool posHasActorWithCollision(double x, double y);
	bool posCanBeTaken(double x, double y, Actor*& collectible);
	int countCensusInArea(double x, double y);
	int getBonus() const;

	// MUTATOR FUNCTIONS
	void addToActors(Actor* na);
	void decreaseNumCrystals();
	void decreaseBonus();
	void completeLevel();

	// CONSTANTS
	static const int RIGHT = 0;
	static const int LEFT = 180;
	static const int UP = 90;
	static const int DOWN = 270;

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
	void removeDeadEntries();
};

#endif // STUDENTWORLD_H_