#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATIONS
Actor::Actor(int ID, int x, int y, int dir, int hp, StudentWorld* sWorld)
	: GraphObject(ID, x, y, dir), m_world(sWorld), collision(true), m_hp(hp), shot(true)
{
	setVisible(true);
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
	entry = getWorld()->findEntryAtPos(static_cast<int>(x), static_cast<int>(y));

	if (entry == nullptr || dynamic_cast<Pit*>(entry) != nullptr)
	{
		moveTo(x, y);
		return true;
	}
	return false;
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

void Actor::changeCollision(bool c)
{
	collision = c;
}

void Actor::changeCanBeShot(bool s)
{
	shot = s;
}

bool Actor::canMove(double x, double y)
{
	Actor* entry = getWorld()->findEntryAtPos(x, y);
	if (entry == nullptr)
	{
		return true;
	}
	return false;
}



// PLAYER IMPLEMENTATIONS
Player::Player(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PLAYER, x, y, right, 20, sWorld), m_peas(20)
{}

void Player::doSomething()
{
	if (!isAlive()) // player is dead
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
				double x = getX();
				double y = getY();

				Pea* np = new Pea(static_cast<int>(x), static_cast<int>(y), dir, getWorld());
				getWorld()->addToActors(np);
				m_peas--;
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
		if (dynamic_cast<Marble*>(entry) != nullptr && entry->isPushable(getDirection()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

int Player::getPeas() const
{
	return m_peas;
}

void Player::addPeas(int p)
{
	m_peas += p;
}

// WALL IMPLEMENTATIONS
Wall::Wall(int x, int y, StudentWorld* sWorld)
	: Actor(IID_WALL, x, y, none, UNDAMAGEABLE, sWorld)
{}

// MARBLE IMPLEMENTATIONS
Marble::Marble(int x, int y, StudentWorld* sWorld)
	: Actor(IID_MARBLE, x, y, none, 10, sWorld)
{}

// PIT IMPLEMENTATIONS
Pit::Pit(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PIT, x, y, none, UNDAMAGEABLE, sWorld)
{
	changeCanBeShot(false);
}

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
	: Actor(IID_PEA, x, y, dir, UNDAMAGEABLE, sWorld)
{
	changeCanBeShot(false);
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
	double x = getX();
	double y = getY();
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

// COLLECTIBLE CLASS IMPLEMENTATIONS
Collectible::Collectible(int ID, int x, int y, int points, StudentWorld* sWorld)
	: Actor(ID, x, y, none, UNDAMAGEABLE, sWorld), m_points(points)
{
	changeCollision(false);
	changeCanBeShot(false);
}

void Collectible::doSomething()
{
	getWorld()->increaseScore(m_points);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	changeHP(0);
}


// CRYSTAL IMPLEMENTATION
Crystal::Crystal(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_CRYSTAL, x, y, CRYSTAL_POINTS, sWorld)
{}

void Crystal::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->isPlayerAt(getX(), getY()))
		{
			Collectible::doSomething();
			getWorld()->decreaseNumCrystals();
		}
	}
}

// EXTRALIFEGOODIE IMPLEMENTATION
ExtraLifeGoodie::ExtraLifeGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_EXTRA_LIFE, x, y, EXTRALIFE_POINTS, sWorld)
{}

void ExtraLifeGoodie::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->isPlayerAt(getX(), getY()))
		{
			Collectible::doSomething();
			getWorld()->incLives();
		}
	}
}

// RESTOREHEALTHGOODIE IMPLEMENTATION
RestoreHealthGoodie::RestoreHealthGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_RESTORE_HEALTH, x, y, RESTOREHEALTH_POINTS, sWorld)
{}

void RestoreHealthGoodie::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->isPlayerAt(getX(), getY()))
		{
			Collectible::doSomething();
			getWorld()->getPlayer()->changeHP(20);
		}
	}
}

// AMMOGOODIE IMPLEMENTATION
AmmoGoodie::AmmoGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_AMMO, x, y, AMMO_POINTS, sWorld)
{}

void AmmoGoodie::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->isPlayerAt(getX(), getY()))
		{
			Collectible::doSomething();
			getWorld()->getPlayer()->addPeas(20);
		}
	}
}

// EXIT IMPLEMENTATION
Exit::Exit(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_EXIT, x, y, FINISH_POINTS, sWorld), revealed(false)
{
	setVisible(false);
}

void Exit::doSomething()
{
	if (isAlive())
	{
		if (!revealed && getWorld()->noMoreCrystals())
		{
			revealed = true;
			setVisible(true);
			getWorld()->playSound(SOUND_REVEAL_EXIT);
		}

		if (revealed)
		{
			if (getWorld()->isPlayerAt(getX(), getY()))
			{
				getWorld()->increaseScore(FINISH_POINTS + getWorld()->getBonus());
				getWorld()->playSound(SOUND_FINISHED_LEVEL);
				getWorld()->completeLevel();
			}
		}
	}
}