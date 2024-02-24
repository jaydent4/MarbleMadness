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


// BASE CLASS
class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y, int dir, int hp, bool visible, bool st, StudentWorld* sWorld);
	StudentWorld* getWorld();
	virtual void doSomething();
	bool hasCollision();
	bool isPushable(int dir);
	bool isAlive();
	bool canBeShot();
	int getHP() const;
	void changeHP(int nhp);
	void damage();

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
private:
	bool canMove(double x, double y);
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

#endif // ACTOR_H_
