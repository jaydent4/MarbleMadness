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

bool Actor::canMoveInDir(int dir)
{
	int nx = getX();
	int ny = getY();

	switch (dir)
	{
	case right: nx++; break;
	case left: nx--; break;
	case up: ny++; break;
	case down: ny--; break;
	}

	if (getWorld()->posHasActorWithCollision(nx, ny))
		return false;
	return true;
}

void Actor::moveInCurrDir()
{
	int dir = getDirection();
	int nx = getX();
	int ny = getY();

	switch (dir)
	{
	case right: nx++; break;
	case left: nx--; break;
	case up: ny++; break;
	case down: ny--; break;
	}

	moveTo(nx, ny);
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
	std::cout << "DAMAGE DONE ";
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
		changeCanBeTaken(false);
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
	moveTo(x, y);
	// CREATE FIND DAMAGEABLE ENTRY
	Actor* entry = nullptr;
	if (getWorld()->posStopsPeas(x, y, entry))
	{
		ety = entry;
		if (getWorld()->isPlayerAt(x, y))
			ety = getWorld()->getPlayer();
		return false;
	}
	return true;

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
{
	changeCanBeTaken(false);
}

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ROBOTS: RAGEBOT, THIEFBOT, REGULARTHIEFBOT, MEANTHIEFBOT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ROBOT IMPLEMENTATIONS
Robot::Robot(int ID, int x, int y, int hp, int dir, int points, StudentWorld* sWorld)
	: Actor(ID, x, y, dir, hp, sWorld), canShoot(true)
{	
	m_currTick = 1;
	m_points = points;
}

// Robot Actions
void Robot::doSomething()
{
	if (isAlive()) // checks if it is alive and can act
	{
		if (canAct())
		{
			if (canShoot) // checks if robots can shoot; if it can, try to shoot
			{
				if (getWorld()->clearShotToPlayerExists(getX(), getY(), getDirection()))
				{
					getWorld()->playSound(SOUND_ENEMY_FIRE);
					Pea* np = new Pea(static_cast<int>(getX()), static_cast<int>(getY()), getDirection(), getWorld());
					getWorld()->addToActors(np);
					return;
				}
			}
			doActivity(); // do any other activity
		}
	}
}

// Determines when Robot can act
bool Robot::canAct()
{
	int tickFreq = (28 - (getWorld()->getLevel())) / 4; // tick frequency
	if (tickFreq < 3) // 3 is fastest rate
		tickFreq = 3; 

	if (m_currTick == tickFreq) // robot is ready to act
	{
		m_currTick = 1;
		return true;
	}
	else // robot cannot act yet
	{
		m_currTick++;
		return false;
	}
}

// In place for ThiefBot
bool Robot::readyToTurn()
{
	return false;
}

// Determines the robot movement option
void Robot::robotMove()
{
	if (canMoveInDir(getDirection())) // move robot one space in the direction it is facing if there is no obstruction
	{
		moveInCurrDir();
	}
	else if (!canMoveInDir(getDirection()) || readyToTurn()) // there is an obstruction, change the direction of the robot
	{
		changeDir();
	}
}

// Determines robot actions when damaged
void Robot::damage()
{
	Actor::damage();
	if (isAlive()) // play sound when damaged
	{
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	}
	else // play different sound when killed and add points to score
	{
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(m_points);
	}
}

// Data Member Change Functions

void Robot::changecanShoot(bool s)
{
	canShoot = s;
}

// RAGEBOT IMPLEMENTATION
RageBot::RageBot(int x, int y, int dir, StudentWorld* sWorld)
	: Robot(IID_RAGEBOT, x, y, RAGEBOT_HP, dir, RAGEBOT_POINTS, sWorld)
{}

// RageBot Actions
void RageBot::doSomething()
{
	Robot::doSomething();
}

// RageBot Specific Action: move
void RageBot::doActivity()
{
	robotMove();
}

// RageBot Movement Options
// When RageBot hits an obstruction, change direction to the opposite direction
void RageBot::changeDir()
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIEFBOT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// THIEFBOT IMPLEMENTATION
ThiefBot::ThiefBot(int ID, int x, int y, StudentWorld* sWorld)
	: Robot(ID, x, y, THIEFBOT_HP, right, THIEFBOT_POINTS, sWorld), stolenItem(nullptr), m_currDistTraveled(0), m_distanceBeforeTurn(randInt(1, 6))
{
	changeFactoryCensus(true);
}

// ThiefBot Actions
void ThiefBot::doSomething()
{
	Robot::doSomething();
}

// ThiefBot Specific Actions: try to steal, move
void ThiefBot::doActivity()
{
	if (!tryToSteal()) // if cannot steal, move
	{
		robotMove();
	}
}

// Attempt to steal the item on the thiefbot
bool ThiefBot::tryToSteal()
{
	Actor* collectible;
	if (getWorld()->posCanBeTaken(getX(), getY(), collectible)) // gets the item from the maze
	{
		if (randInt(1, 10) == 1) // 1/10 chance
		{
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			stolenItem = collectible->steal(); // steals item
			return true;
		}
	}
	return false;
}

// ThiefBot Movement Option
// When ThiefBot is blocked by an obstruction or is ready to turn, pick a new random direction
void ThiefBot::changeDir()
{
	m_distanceBeforeTurn = randInt(1, 6); // determine the distance of the next direction
	m_currDistTraveled = 0; // reset distance traveled

	/*
	DIRECTIONS FROM 1-4:
	1: right
	2: left
	3: up
	4: down
	*/

	int dir = randInt(1, 4);
	int startingDir = dir; // saves the original starting int if ThiefBot is completely obstructed
	switch (dir)
	{
	case 1: startingDir = right; break;
	case 2: startingDir = left; break;
	case 3: startingDir = up; break;
	case 4: startingDir = down; break;
	}
	int decodedDir = -1; // direction that is manipulated

	std::set<int> visitedDir; // holds directions already checked

	while (visitedDir.size() != 4) // while all directions have not been visited
	{
		switch (dir)
		{
		case 1: decodedDir = right; break;
		case 2: decodedDir = left; break;
		case 3: decodedDir = up; break;
		case 4: decodedDir = down; break;
		}

		if (visitedDir.find(dir) != visitedDir.end()) // direction has already been considered
		{
			dir = randInt(1, 4);
		}
		else if (!canMoveInDir(decodedDir)) // movement in direction is not possible
		{
			visitedDir.insert(dir); // inserts direction into set of directions
			dir = randInt(1, 4); // choses new direction
		}
		else
		{
			setDirection(decodedDir); // found viable direction
			return;
		}
	}
	setDirection(startingDir); // if no direction is feasible, set direction to first one
}

// Determines whether ThiefBot can turn
bool ThiefBot::readyToTurn()
{
	if (m_currDistTraveled == m_distanceBeforeTurn) // traveled distance limit
	{
		m_currDistTraveled = 0; // reset distance traveled
		return true;
	}
	else // increemnt distance traveled
	{
		m_currDistTraveled++;
		return false;
	}
}

// ThiefBot Movement
void ThiefBot::robotMove()
{
	Robot::robotMove();
	if (stolenItem != nullptr) // move item with it if holding an item
		stolenItem->moveTo(getX(), getY());
}

// Damage ThiefBot
void ThiefBot::damage()
{
	Robot::damage();
	if (!isAlive())
	{
		if (isHoldingItem()) // drop item if killed
		{
			stolenItem->setVisible(true);
			stolenItem->changeCanBeTaken(true);
		}
	}
}

// Determine if ThiefBot is holding/stole an item
bool ThiefBot::isHoldingItem()
{
	return (stolenItem != nullptr);
}


// REGULARTHIEFBOT IMMPLEMENTATIONS
RegularThiefBot::RegularThiefBot(int x, int y, StudentWorld* sWorld)
	: ThiefBot(IID_THIEFBOT, x, y, sWorld)
{
	changecanShoot(false);
}

// MEAN THIEFBOT IMPLEMENTATION
MeanThiefBot::MeanThiefBot(int x, int y, StudentWorld* sWorld)
	: ThiefBot(IID_MEAN_THIEFBOT, x, y, sWorld)
{}

// THIEFBOTFACTORY IMPLEMENTATION
ThiefBotFactory::ThiefBotFactory(int x, int y, ProductType type, StudentWorld* sWorld)
	: Actor(IID_ROBOT_FACTORY, x, y, none, UNDAMAGEABLE, sWorld), m_tbType(type)
{}

void ThiefBotFactory::doSomething()
{
	if (census())
	{
		if (randInt(1, 50) == 1) // 1/50 chance
		{
			getWorld()->playSound(SOUND_ROBOT_BORN);
			switch (m_tbType)
			{
			case REGULAR:
			{
				RegularThiefBot* nrtb = new RegularThiefBot(getX(), getY(), getWorld());
				getWorld()->addToActors(nrtb);
				break;
			}
			case MEAN:
			{
				MeanThiefBot* nmtb = new MeanThiefBot(getX(), getY(), getWorld());
				getWorld()->addToActors(nmtb);
				break;
			}
			}
		}
	}
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
			if (getWorld()->getThiefBotAtPos(centerX + dx, centerY + dy) != nullptr)
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

void ThiefBotFactory::damage()
{
	Actor* thiefonfac = getWorld()->getThiefBotAtPos(getX(), getY());
	if (thiefonfac != nullptr)
		thiefonfac->damage();
}


