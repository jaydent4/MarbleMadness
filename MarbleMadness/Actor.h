#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "level.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// CONSTANTS
const int UNDAMAGEABLE = 999;
const int PEA_DAMAGE = 2;
const int CRYSTAL_POINTS = 50;
const int EXTRALIFE_POINTS = 1000;
const int RESTOREHEALTH_POINTS = 500;
const int AMMO_POINTS = 100;
const int FINISH_POINTS = 2000;

const int RAGEBOT_HP = 10;
const int RAGEBOT_POINTS = 100;
const int THIEFBOT_HP = 5;
const int THIEFBOT_POINTS = 10;


// BASE CLASS
class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y, int dir, int hp, StudentWorld* sWorld);
	StudentWorld* getWorld();
	virtual void doSomething();
	bool hasCollision();
	bool isPushable(int dir);
	bool isAlive();
	bool canBeShot();
	int getHP() const;
	void changeHP(int nhp);
	virtual void damage();
	virtual bool canMove(double x, double y);
	virtual bool canBeTaken();
	Actor* steal();
	virtual bool isStolen();
	void changeCanBeTaken(bool t);
	bool isPartOfFactoryCensus();
	void changeFactoryCensus(bool fc);


	// HELPER FUNCTIONS
	void changeCollision(bool c);
	void changeCanBeShot(bool s);
private:
	StudentWorld* m_world;
	bool collision;
	bool shot;
	bool taken;
	bool stolen;
	bool factoryCensus;
	int m_hp;
};


// DERIVED CLASSES

class Player : public Actor
{
public:
	Player(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
	int getPeas() const;
	void addPeas(int p);
private:
	virtual bool canMove(double x, double y);
	int m_peas;
};

class Wall : public Actor
{
public:
	Wall(int x, int y, StudentWorld* sWorld);
private:
};

class Marble : public Actor
{
public:
	Marble(int x, int y, StudentWorld* sWorld);
private:
};

class Pit : public Actor
{
public:
	Pit(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
private:
};

class Pea : public Actor
{
public:
	Pea(int x, int y, int dir, StudentWorld* sWorld);
	virtual void doSomething();
private:
	bool moveInDirection(Actor*& ety);
};

class ThiefBotFactory : public Actor
{
public:
	enum ProductType { REGULAR, MEAN };
	ThiefBotFactory(int x, int y, ProductType type, StudentWorld* sWorld);
	void doSomething();
private:
	bool census();
	ProductType getType() const;
	ProductType m_tbType;
};

// COLLECTIBLE CLASSES
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

class Crystal : public Collectible
{
public:
	Crystal(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

class ExtraLifeGoodie : public Collectible
{
public:
	ExtraLifeGoodie(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

class RestoreHealthGoodie : public Collectible
{
public:
	RestoreHealthGoodie(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

class AmmoGoodie : public Collectible
{
public:
	AmmoGoodie(int x, int y, StudentWorld* sWorld);
	virtual void doActivity();
};

class Exit : public Collectible
{
public:
	Exit(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
	virtual void doActivity();
private:
	bool revealed;
};

// ROBOT CLASSES
class Robot : public Actor
{
public:
	Robot(int ID, int x, int y, int hp, int dir, int points, StudentWorld* sWorld);
	void doSomething();
	bool canAct();
	virtual void robotMove();
	virtual void movementPattern() = 0;
	virtual void damage();
private:
	int m_currTick;
	int m_points;
};

class RageBot : public Robot
{
public:
	RageBot(int x, int y, int dir, StudentWorld* sWorld);
	virtual void doSomething();
	virtual void movementPattern();
};

class ThiefBot : public Robot
{
public:
	ThiefBot(int ID, int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
	virtual void doActivity();
	virtual void robotMove();
	virtual void movementPattern();
	bool isHoldingItem();
	bool readyToTurn();
	virtual bool tryToSteal();
	virtual void damage();
private:
	Actor* stolenItem;
	int m_distanceBeforeTurn;
	int m_currDistTraveled;
};

class RegularThiefBot : public ThiefBot
{
public:
	RegularThiefBot(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
};

#endif // ACTOR_H_
