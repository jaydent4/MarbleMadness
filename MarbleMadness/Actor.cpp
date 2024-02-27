#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATIONS
Actor::Actor(int ID, int x, int y, int dir, int hp, StudentWorld* sWorld)
	: GraphObject(ID, x, y, dir), m_world(sWorld), collision(true), m_hp(hp), shot(true), taken(false), stolen(false), factoryCensus(false)
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

bool Actor::canBeTaken()
{
	return taken;
}

void Actor::changeCanBeTaken(bool t)
{
	taken = t;
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
	if (entry != nullptr && entry->hasCollision() || getWorld()->isPlayerAt(x,y))
	{
		return false;
	}
	return true;
}

Actor* Actor::steal()
{
	if (canBeTaken())
	{
		stolen = true;
		setVisible(false);
		return this;
	}
	return nullptr;
}

bool Actor::isStolen()
{
	return stolen;
}

bool Actor::isPartOfFactoryCensus()
{
	return factoryCensus;
}

void Actor::changeFactoryCensus(bool fc)
{
	factoryCensus = fc;
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
	if (Actor::canMove(x, y))
	{
		return true;
	}

	Actor* entry = getWorld()->findEntryAtPos(x, y);
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
	moveTo(x, y);
	// CREATE FIND DAMAGEABLE ENTRY
	Actor* entry = getWorld()->findEntryAtPos(x, y);
	if (entry == nullptr || (!(entry->canBeShot()) && !getWorld()->isPlayerAt(x, y)))
	{
		return true;
	}
	ety = entry;
	if (getWorld()->isPlayerAt(x, y))
		ety = getWorld()->getPlayer();
	return false;
}

// COLLECTIBLE CLASS IMPLEMENTATIONS
Collectible::Collectible(int ID, int x, int y, int points, StudentWorld* sWorld)
	: Actor(ID, x, y, none, UNDAMAGEABLE, sWorld), m_points(points)
{
	changeCollision(false);
	changeCanBeShot(false);
	changeCanBeTaken(true);
}

void Collectible::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->isPlayerAt(getX(), getY()) && !isStolen())
		{
			getWorld()->increaseScore(m_points);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			doActivity();
			changeHP(0);
		}
	}
}

// CRYSTAL IMPLEMENTATION
Crystal::Crystal(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_CRYSTAL, x, y, CRYSTAL_POINTS, sWorld)
{}

void Crystal::doActivity()
{
	getWorld()->decreaseNumCrystals();
}

// EXTRALIFEGOODIE IMPLEMENTATION
ExtraLifeGoodie::ExtraLifeGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_EXTRA_LIFE, x, y, EXTRALIFE_POINTS, sWorld)
{}

void ExtraLifeGoodie::doActivity()
{
	getWorld()->incLives();
}

// RESTOREHEALTHGOODIE IMPLEMENTATION
RestoreHealthGoodie::RestoreHealthGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_RESTORE_HEALTH, x, y, RESTOREHEALTH_POINTS, sWorld)
{}

void RestoreHealthGoodie::doActivity()
{
	getWorld()->getPlayer()->changeHP(20);
}

// AMMOGOODIE IMPLEMENTATION
AmmoGoodie::AmmoGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_AMMO, x, y, AMMO_POINTS, sWorld)
{}

void AmmoGoodie::doActivity()
{
	getWorld()->getPlayer()->addPeas(20);
}

// EXIT IMPLEMENTATION
Exit::Exit(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_EXIT, x, y, FINISH_POINTS, sWorld), revealed(false)
{
	setVisible(false);
	changeCanBeTaken(false);
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

void Exit::doActivity() // Exit does not use doActivity
{}

// ROBOT IMPLEMENTATIONS
Robot::Robot(int ID, int x, int y, int hp, int dir, int points, StudentWorld* sWorld)
	: Actor(ID, x, y, dir, hp, sWorld)
{	
	m_currTick = 1;
	m_points = points;
}

void Robot::doSomething()
{

}

bool Robot::canAct()
{
	int tickFreq = (28 - (getWorld()->getLevel())) / 4; // tick frequency
	if (tickFreq < 3) // 3 is fastest rate
		tickFreq = 3; 

	if (m_currTick == tickFreq)
	{
		m_currTick = 1;
		return true;
	}
	else
	{
		m_currTick++;
		return false;
	}
}

void Robot::robotMove()
{
	int dir = getDirection();
	double x = getX();
	double y = getY();
	double dx = 0;
	double dy = 0;

	switch (dir)
	{
	case right: dx++; break;
	case left: dx--; break;
	case up: dy++; break;
	case down: dy--; break;
	}

	Actor* entry = getWorld()->findEntryAtPos(x + dx, y + dy);
	if (entry != nullptr && entry->hasCollision())
	{
		movementPattern();
	}
	else
	{
		moveTo(x + dx, y + dy);
	}
}

void Robot::damage()
{
	Actor::damage();
	if (isAlive())
	{
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	}
	else
	{
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(m_points);
	}
}

// RAGEBOT IMPLEMENTATION
RageBot::RageBot(int x, int y, int dir, StudentWorld* sWorld)
	: Robot(IID_RAGEBOT, x, y, RAGEBOT_HP, dir, RAGEBOT_POINTS, sWorld)
{}

void RageBot::doSomething()
{
	if (isAlive())
	{
		if (canAct())
		{
			if (getWorld()->clearShotToPlayerExists(getX(), getY(), getDirection()))
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				Pea* np = new Pea(static_cast<int>(getX()), static_cast<int>(getY()), getDirection(), getWorld());
				getWorld()->addToActors(np);
			}
			else
			{
				robotMove();
			}
		}
	}
}

void RageBot::movementPattern()
{
	int dir = getDirection();
	switch (dir)
	{
	case up: setDirection(down); break;
	case down: setDirection(up); break;
	case right: setDirection(left); break;
	case left: setDirection(right); break;
	}
}

// THIEFBOT IMPLEMENTATION
ThiefBot::ThiefBot(int ID, int x, int y, StudentWorld* sWorld)
	: Robot(ID, x, y, THIEFBOT_HP, right, THIEFBOT_POINTS, sWorld), stolenItem(nullptr), m_currDistTraveled(0), m_distanceBeforeTurn(randInt(1, 6))
{
	changeFactoryCensus(true);
}

void ThiefBot::doSomething()
{
	if (isAlive())
	{
		if (canAct())
		{
			doActivity();
			if (!tryToSteal())
			{
				robotMove();
			}
		}
	}
}

bool ThiefBot::tryToSteal()
{
	Actor* entry = getWorld()->findEntryAtPos(getX(), getY());
	if (entry != nullptr && entry->canBeTaken() && !(entry->isStolen()) && !isHoldingItem())
	{
		if (randInt(1, 10) == 1)
		{
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			stolenItem = entry->steal();
			return true;
		}
	}
	return false;
}

void ThiefBot::doActivity()
{}

void ThiefBot::robotMove()
{
	int dx = 0;
	int dy = 0;
	int dir = getDirection();
	switch (dir)
	{
	case right: dx++; break;
	case left: dx--; break;
	case up: dy++; break;
	case down: dy--; break;
	}


	if (readyToTurn() || !canMove(getX() + dx, getY() + dy))
		movementPattern();
	else
	{
		moveTo(getX() + dx, getY() + dy);
		if (isHoldingItem())
			stolenItem->moveTo(getX() + dx, getY() + dy);
	}
}

void ThiefBot::damage()
{
	Robot::damage();
	if (!isAlive())
	{
		stolenItem->setVisible(true);
		stolenItem->changeCanBeTaken(true);
	}
}

bool ThiefBot::isHoldingItem()
{
	return (stolenItem != nullptr);
}

bool ThiefBot::readyToTurn()
{
	if (m_currDistTraveled == m_distanceBeforeTurn)
	{
		m_currDistTraveled = 0;
		return true;
	}
	else
	{
		m_currDistTraveled++;
		return false;
	}
}

void ThiefBot::movementPattern()
{
	m_distanceBeforeTurn = randInt(1, 6);
	m_currDistTraveled = 0;

	/*
	1: right
	2: left
	3: up
	4: down
	*/
	int dir;
	int decodedDir;
	std::set<int>visitedDirs;
	int dx = 0;
	int dy = 0;
	while (visitedDirs.size() != 4)
	{
		dx = 0;
		dy = 0;
		dir = randInt(1, 4);
		switch (dir)
		{
		case 1: dx++; break;
		case 2: dx--; break;
		case 3: dy++; break;
		case 4: dy--; break;
		}
		
		if (visitedDirs.find(dir) != visitedDirs.end())
			continue;

		if (!canMove(getX() + dx, getY() + dy))
		{
			visitedDirs.insert(dir);
		}
		else
		{
			switch (dir)
			{
			case 1: decodedDir = right; break;
			case 2: decodedDir = left; break;
			case 3: decodedDir = up; break;
			case 4: decodedDir = down; break;
			}
			setDirection(decodedDir);
			break;
		}
	}
}

// REGULARTHIEFBOT IMMPLEMENTATIONS
RegularThiefBot::RegularThiefBot(int x, int y, StudentWorld* sWorld)
	: ThiefBot(IID_THIEFBOT, x, y, sWorld)
{}

void RegularThiefBot::doSomething()
{
	ThiefBot::doSomething();
}

// THIEFBOTFACTOR IMPLEMENTATION
ThiefBotFactory::ThiefBotFactory(int x, int y, ProductType type, StudentWorld* sWorld)
	: Actor(IID_ROBOT_FACTORY, x, y, none, UNDAMAGEABLE, sWorld), m_tbType(type)
{}

void ThiefBotFactory::doSomething()
{
	if (census())
	{
		if (randInt(1, 50) == 1) // 1/50 chance
		{
			switch (getType())
			{
			case REGULAR:
				RegularThiefBot* nrtb = new RegularThiefBot(getX(), getY(), getWorld());
				getWorld()->addToActors(nrtb);
				break;
			// ADD CASE MEAN
			}
		}
	}
}

ThiefBotFactory::ProductType ThiefBotFactory::getType() const
{
	return m_tbType;
}

bool ThiefBotFactory::census()
{
	double centerX = getX();
	double centerY = getY();
	int numThiefBot = 0;
	Actor* entry;

	for (int dy = 3; dy >= -3; dy--)
	{
		for (int dx = -3; dx <= 3; dx++)
		{
			if (dx == 0 && dy == 0)
			{
				entry = getWorld()->getThiefBotAtPos(centerX, centerY);
				if (entry != nullptr)
					return false;
				else
					continue;
			}
			entry = getWorld()->findEntryAtPos(centerX + dx, centerY + dy);
			if (entry != nullptr && entry->isPartOfFactoryCensus())
			{
				numThiefBot++;
			}
			if (numThiefBot >= 3)
			{
				return false;
			}
		}
	}
	return true;
}



