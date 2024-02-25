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
	void damage();
	virtual bool canMove(double x, double y);


	// HELPER FUNCTIONS
	void changeCollision(bool c);
	void changeCanBeShot(bool s);
private:
	StudentWorld* m_world;
	bool collision;
	bool shot;
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
	int m_hp;
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

// COLLECTIBLE CLASSES
class Collectible : public Actor
{
public:
	Collectible(int ID, int x, int y, int points, StudentWorld* sWorld);
	virtual void doSomething();
	virtual void doActivity() = 0;
private:
	int m_points;
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

#endif // ACTOR_H_
