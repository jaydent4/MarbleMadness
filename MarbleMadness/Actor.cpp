#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATIONS
Actor::Actor(int ID, int x, int y, int dir, bool visible, StudentWorld* sWorld, Level::MazeEntry type)
	: GraphObject(ID, x, y, dir), m_world(sWorld), collision(true)
{
	setVisible(visible);
}

StudentWorld* Actor::getWorld()
{
	return m_world;
}

void Actor::doSomething() {}

bool Actor::hasCollision()
{
	return collision;
}

// PLAYER IMPLEMENTATIONS
Player::Player(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PLAYER, x, y, right, true, sWorld, Level::player), m_hp(20), m_peas(20)
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
		case KEY_PRESS_LEFT:
			if (canMove(getX() - 1, getY()))
			{
				moveTo(getX() - 1, getY());
			}
			setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (canMove(getX() + 1, getY()))
			{
				moveTo(getX() + 1, getY());
			}
			setDirection(right);
			break;
		case KEY_PRESS_DOWN:
			if (canMove(getX(), getY() - 1))
			{
				moveTo(getX(), getY() - 1);
			}
			setDirection(down);
			break;
		case KEY_PRESS_UP:
			if (canMove(getX(), getY() + 1))
			{
				moveTo(getX(), getY() + 1);
			}
			setDirection(up);
			break;
		}
	}
}

bool Player::canMove(double x, double y)
{
	Actor* entry = getWorld()->findEntryAtPos(x, y);
	if (entry == nullptr || !(entry->hasCollision())) // nullptr is empty, if entry exists, check if it has collision
	{
		if (true) // UPDATE FOR MARBLE
			return true;
	}
	return false;
}

int Player::getHP() const
{
	return m_hp;
}

int Player::getPeas() const
{
	return m_peas;
}

// WALL IMPLEMENTATIONS
Wall::Wall(int x, int y, StudentWorld* sWorld)
	: Actor(IID_WALL, x, y, none, true, sWorld, Level::wall)
{}

