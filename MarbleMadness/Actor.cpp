#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATIONS
Actor::Actor(int ID, int x, int y, int dir, bool visible, StudentWorld* sWorld)
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

bool Actor::isPushable(int dir)
{
	if (pushable)
	{
		Actor* entry = nullptr;
		int x = getX();
		int y = getY();
		switch (dir)
		{
		case up:
			y++;
			break;
		case down:
			y--;
			break;
		case right:
			x++;
			break;
		case left:
			x--;
			break;
		}
		entry = getWorld()->findEntryAtPos(x, y);

		if (entry != nullptr && entry->hasCollision())
		{
			return false;
		}
		moveTo(x, y);
		return true;
	}
	return false;
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
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (canMove(getX() - 1, getY()))
			{
				moveTo(getX() - 1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (canMove(getX() + 1, getY()))
			{
				moveTo(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (canMove(getX(), getY() - 1))
			{
				moveTo(getX(), getY() - 1);
			}
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (canMove(getX(), getY() + 1))
			{
				moveTo(getX(), getY() + 1);
			}
			break;
		}
	}
}

bool Player::canMove(double x, double y)
{
	Actor* entry = getWorld()->findEntryAtPos(x, y);
	if (entry == nullptr)
	{
		return true;
	}

	if (entry->hasCollision())
	{
		if (dynamic_cast<Marble*>(entry) != nullptr)
		{
			return entry->isPushable(getDirection());
		}
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
	: Actor(IID_WALL, x, y, none, true, sWorld)
{}

// MARBLE IMPLEMENTATIONS
Marble::Marble(int x, int y, StudentWorld* sWorld)
	: Actor(IID_MARBLE, x, y, none, true, sWorld), m_hp(10)
{}
