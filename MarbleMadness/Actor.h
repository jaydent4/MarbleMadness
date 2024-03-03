#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "Level.h"
#include <algorithm>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// CONSTANTS


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ACTOR CLASSES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// BASE CLASS
class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y, int dir, int hp, StudentWorld* sWorld);

	// ACTOR ACTION FUNCTIONS
	virtual void doSomething(); // Actor Action
	void damage(); // damage the actor
	Actor* steal(); // steal Actor
	bool canMoveInDir(int dir); // Check if Actor can move in given direction
	void moveInCurrDir(); // Move in current direction
	bool isPushableInDir(int dir); // Check if Marble is pushable in a specified direction
	

	// IN-GAME ACCESSOR FUNCTIONS
	int getHP() const; // Get HP of Actor
	StudentWorld* getWorld() const; // Get the World that Actor is in

	// IN-GAME MUTATOR FUNCTIONS
	void changeHP(int nhp); // Change the HP of Actor
	void changeStolen(bool s); // Change Actor status to stolen

	// IN-GAME STATUS FUNCTIONS
	bool isAlive(); // Check if Actor is alive
	bool isStolen(); // Check if Actor is stolen


	// INITIAL TYPE FUNCTIONS
	bool hasCollision(); // Check if Actor can be walked into
	bool canBeShot(); // Check if peas collide with Actor
	bool canBeTaken(); // Check if Actor can be stolen
	bool isPartOfFactoryCensus(); // Check if Actor is part of the thief bot factory census
	bool isPushable(); // Check if the Actor can be pushed
	bool canSwallow(); // Check if the Actor can swallow a Marble

	// INITIAL TYPE MUTATOR FUNCTIONS
	void changeCollision(bool c);
	void changeCanBeShot(bool s);
	void changeFactoryCensus(bool fc);
	void changePushable(bool p);
	void changeSwallow(bool s);
	void changeCanBeTaken(bool t);
	
	// ACTOR CONSTANTS
	static const int UNDAMAGEABLE = 999;
	static const int PEA_DAMAGE = 2;
	static const int CRYSTAL_POINTS = 50;
	static const int EXTRALIFE_POINTS = 1000;
	static const int RESTOREHEALTH_POINTS = 500;
	static const int AMMO_POINTS = 100;
	static const int FINISH_POINTS = 2000;
	static const int RAGEBOT_HP = 10;
	static const int RAGEBOT_POINTS = 100;
	static const int THIEFBOT_HP = 5;
	static const int REGULAR_THIEFBOT_POINTS = 10;
	static const int MEAN_THIEFBOT_POINTS = 20;


private:
	StudentWorld* m_world;
	bool collision;
	bool shot;
	bool taken;
	bool stolen;
	bool factoryCensus;
	bool pushable;
	bool swallow;
	int m_hp;
};


// DERIVED CLASSES

class Player : public Actor
{
public:
	Player(int x, int y, StudentWorld* sWorld);
	virtual void doSomething(); // Move the player, shoot pea, end the level
	int getPeas() const; // Get the number of peas
	void addPeas(int p); // Add more peas
private:
	bool canMove(double x, double y); // Check if the Player can move to (x, y)
	int m_peas;
};

// Blocks Actors
class Wall : public Actor
{
public:
	Wall(int x, int y, StudentWorld* sWorld);
private:
};

// Blocks actors, can be pushed, swallowed by pits, and destroyed
class Marble : public Actor
{
public:
	Marble(int x, int y, StudentWorld* sWorld);
private:
};

// Blocks actors, can be removed if swallow marble
class Pit : public Actor
{
public:
	Pit(int x, int y, StudentWorld* sWorld);
	virtual void doSomething(); // Check if a marble in the same position as a pit; if it is, remove both
private:
};

// Hits Actors and deals damage 
class Pea : public Actor
{
public:
	Pea(int x, int y, int dir, StudentWorld* sWorld);
	virtual void doSomething(); // Continuously move in a direction
private:
	bool moveInDirection(Actor*& ety);
};

// Spawns new thief bots
class ThiefBotFactory : public Actor
{
public:
	ThiefBotFactory(int x, int y, bool mean, StudentWorld* sWorld);
	void doSomething(); // check census, make new thief bots if the conditions follow
	virtual void damage(); // redirect damage to the thief bot on the factory
private:
	bool census();
	bool outputMean;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COLLECTIBLE CLASSES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// BASE CLASS
class Collectible : public Actor
{
public:
	Collectible(int ID, int x, int y, int points, StudentWorld* sWorld);
	virtual void doSomething();
	virtual void doActivity() = 0;
private:
	int m_points;
	bool stolen;
};

// CRYSTAL: COLLECT TO COMPLETE LEVEL
class Crystal : public Collectible
{
public:
	Crystal(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

// EXTRALIFEGOODIE: INCREASES LIVES
class ExtraLifeGoodie : public Collectible
{
public:
	ExtraLifeGoodie(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

// RESTOREHEALTHGOODIE: SETS PLAYER'S HEALTH TO 20
class RestoreHealthGoodie : public Collectible
{
public:
	RestoreHealthGoodie(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

// AMMOGOODIE: INCREASES PLAYER'S PEAS BY 20
class AmmoGoodie : public Collectible
{
public:
	AmmoGoodie(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

// EXIT: REVEAL, COMPELETE LEVEL
class Exit : public Collectible
{
public:
	Exit(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
	virtual void doActivity();
private:
	bool revealed;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ROBOT CLASSES
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// BASE CLASS
class Robot : public Actor
{
public:
	Robot(int ID, int x, int y, int hp, int dir, int points, StudentWorld* sWorld);
	virtual void doSomething(); // performs some action (specified by type of robot)
	bool canAct(); // Determines when robot can act
	virtual bool readyToTurn(); // Always true for RageBots, Determined through a function for thiefbots
	virtual void robotMove(); // moves robot
	virtual void damage(); // damages robot

	// DATA MEMBER CHANGE FUNCTIONS
	virtual void changecanShoot(bool s); // changes canShoot to s

	// PURE VIRTUAL FUNCTIONS
	virtual void changeDir() = 0; // direction change (specified by type of robot)
	virtual void doActivity() = 0; // activity done by robot (specified by type of robot)

private:
	int m_currTick;
	int m_points;
	bool canShoot;
};

// RAGEBOT: SHOOT, MOVE
class RageBot : public Robot
{
public:
	RageBot(int x, int y, int dir, StudentWorld* sWorld);
	virtual void doSomething(); // shoots, moves
	virtual void changeDir(); // changes direction to opposite direction
	virtual void doActivity(); // moves
};

// BASE CLASS
class ThiefBot : public Robot
{
public:
	ThiefBot(int ID, int x, int y, int points, StudentWorld* sWorld);
	virtual void doSomething(); // shoots, steals, moves
	virtual bool tryToSteal(); // attempts to steal an item
	virtual void changeDir(); // change direction to random direction
	virtual bool readyToTurn(); // determines if ThiefBot is ready to turn based on how far it traveled
	virtual void robotMove(); // moves ThiefBot, moves stolen item with it
	virtual void damage(); // damages ThiefBot, drops stolen item
	bool isHoldingItem(); // determine if robot is holding an item

private:
	virtual void doActivity(); // try to steal, move
	Actor* stolenItem;
	int m_distanceBeforeTurn;
	int m_currDistTraveled;
};

// STEAL, MOVE,
class RegularThiefBot : public ThiefBot
{
public:
	RegularThiefBot(int x, int y, StudentWorld* sWorld);
};

// SHOOT, STEAL, MOVE
class MeanThiefBot : public ThiefBot
{
public:
	MeanThiefBot(int x, int y, StudentWorld* sWorld);
};

#endif // ACTOR_H_
