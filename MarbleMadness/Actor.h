#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "level.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// BASE CLASS
class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y, int dir, bool visible, StudentWorld* sWorld, Level::MazeEntry type);
	StudentWorld* getWorld();
	virtual void doSomething();
	bool hasCollision();
private:
	StudentWorld* m_world;
	bool collision;
};


// DERIVED CLASSES
class Player : public Actor
{
public:
	Player(int x, int y, StudentWorld* sWorld);
	virtual void doSomething();
	int getHP() const;
	int getPeas() const;
private:
	bool canMove(double x, double y);
	int m_hp;
	int m_peas;
};

class Wall : public Actor
{
public:
	Wall(int x, int y, StudentWorld* sWorld);
private:
};


#endif // ACTOR_H_
