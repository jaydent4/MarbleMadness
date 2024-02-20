#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATIONS
Actor::Actor(int ID, int x, int y, int dir, bool visible, StudentWorld* sWorld)
	: GraphObject(ID, x, y, dir), m_world(sWorld)
{
	setVisible(visible);
}

StudentWorld* Actor::getWorld()
{
	return m_world;
}

// PLAYER IMPLEMENTATIONS
Player::Player(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PLAYER, x, y, right, true, sWorld), m_hp(20), m_peas(20)
{}

void Player::doSomething()
{
	if (m_hp <= 0) // player is dead
		return;

	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT: std::cout << "LEFT"; moveTo(getX() - 1, getY()); break;
		case KEY_PRESS_RIGHT: moveTo(getX() + 1, getY()); break;
		case KEY_PRESS_DOWN: moveTo(getX(), getY() - 1); break;
		case KEY_PRESS_UP: moveTo(getX(), getY() + 1); break;
		}
	}
}

int Player::getHP() const
{
	return m_hp;
}

// WALL IMPLEMENTATIONS
Wall::Wall(int x, int y, StudentWorld* sWorld)
	: Actor(IID_WALL, x, y, none, true, sWorld)
{}

