#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y, int dir, bool visible, StudentWorld* sWorld);
	StudentWorld* getWorld();
private:
	StudentWorld* m_world;
};

class Player : public Actor
{
public:
	Player(int x, int y, StudentWorld* sWorld);
	void doSomething();
	int getHP() const;
private:
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
