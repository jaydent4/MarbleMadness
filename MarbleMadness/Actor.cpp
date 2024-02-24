#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATIONS
Actor::Actor(int ID, int x, int y, int dir, int hp, bool visible, bool st, StudentWorld* sWorld)
	: GraphObject(ID, x, y, dir), m_world(sWorld), collision(true), m_hp(hp), shot(st)
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
	Actor* entry = nullptr;
	double x = getX();
	double y = getY();
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

	if (entry != nullptr && entry->hasCollision() && dynamic_cast<Pit*>(entry) == nullptr)
	{
		return false;
	}
	moveTo(x, y);
	return true;
}

bool Actor::isAlive()
{
	return (getHP() > 0);
}

int Actor::getHP() const
{
	return m_hp;
}

void Actor::changeHP(int nhp)
{
	m_hp = nhp;
}

void Actor::damage()
{
	if (m_hp != UNDAMAGEABLE)
		m_hp -= PEA_DAMAGE;
}

bool Actor::canBeShot()
{
	return shot;
}



// PLAYER IMPLEMENTATIONS
Player::Player(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PLAYER, x, y, right, 20, true, true, sWorld), m_peas(20)
{}

void Player::doSomething()
{
	if (false) // player is dead
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
		case KEY_PRESS_SPACE:
			if (m_peas > 0)
			{
				int dir = getDirection();
				int x = getX();
				int y = getY();

				Pea* np = new Pea(x, y, dir, getWorld());
				getWorld()->addToActors(np);
			}
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

int Player::getPeas() const
{
	return m_peas;
}

// WALL IMPLEMENTATIONS
Wall::Wall(int x, int y, StudentWorld* sWorld)
	: Actor(IID_WALL, x, y, none, UNDAMAGEABLE, true, true, sWorld)
{}

// MARBLE IMPLEMENTATIONS
Marble::Marble(int x, int y, StudentWorld* sWorld)
	: Actor(IID_MARBLE, x, y, none, 10, true, true, sWorld)
{}

// PIT IMPLEMENTATIONS
Pit::Pit(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PIT, x, y, none, UNDAMAGEABLE, true, false, sWorld)
{}

void Pit::doSomething()
{
	if (isAlive())
	{
		Marble* pm = getWorld()->getMarbleAtPos(getX(), getY());
		if (pm != nullptr)
		{
			changeHP(0); // changes to dead
			pm->changeHP(0); // changes to dead;
		}
	}
}

// PEA IMPLEMENTATIONS
Pea::Pea(int x, int y, int dir, StudentWorld* sWorld)
	: Actor(IID_PEA, x, y, dir, UNDAMAGEABLE, true, false, sWorld)
{
	setVisible(true);
}

void Pea::doSomething()
{
	if (isAlive())
	{
		Actor* entry = nullptr;
		if (!moveInDirection(entry))
		{
			entry->damage();
			changeHP(0);
		}
	}
}

bool Pea::moveInDirection(Actor*& ety)
{
	// get coordinates and direction
	int x = getX();
	int y = getY();
	int dir = getDirection();

	// determine new position of the pea
	switch (dir)
	{
	case up: y++; break;
	case down: y--; break;
	case right: x++; break;
	case left: x--; break;
	}

	// check if pea goes through or stops at an entry/actor
	Actor* entry = getWorld()->findEntryAtPos(x, y);
	if (entry == nullptr || !(entry->canBeShot()))
	{
		moveTo(x, y);
		return true;
	}
	ety = entry;
	return false;
}