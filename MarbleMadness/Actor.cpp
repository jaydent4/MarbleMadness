#include "Actor.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ACTOR: PLAYER, WALL, MARBLE, PIT, PEA, COLLECTIBLE, ROBOT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ACTOR IMPLEMENTATIONS
Actor::Actor(int ID, int x, int y, int dir, int hp, StudentWorld* sWorld)
	: GraphObject(ID, x, y, dir), m_world(sWorld), collision(true), m_hp(hp), shot(true), taken(false), stolen(false), factoryCensus(false), pushable(false), swallow(false)
{
	setVisible(true);
}

// ACTOR ACTION FUNCTIONS
// Actor Action
void Actor::doSomething() {} // base actor has no action

// Damage Actor
void Actor::damage()
{
	if (m_hp != UNDAMAGEABLE) // if HP is set to UNDAMAGEABLE, do not deal damage (UNDAMAGEABLE is set to 999, which no other actor has)
		m_hp -= PEA_DAMAGE;
}

// Steal Actor (Collectible)
Actor* Actor::steal()
{
	if (canBeTaken()) // only collectible items can be stolen
	{
		stolen = true; // ensures that player or thief bot can take the item
		setVisible(false);
		return this; // return the item that is stolen
	}
	return nullptr; //  return nothing if the item cannot be stolen
}

// Check if the actor can move in a direction
bool Actor::canMoveInDir(int dir)
{
	double nx = getX();
	double ny = getY();

	switch (dir)
	{
	case right: nx++; break;
	case left: nx--; break;
	case up: ny++; break;
	case down: ny--; break;
	}

	if (getWorld()->posHasActorWithCollision(nx, ny)) // actor cannot move unless the space is empty or the actor at the current space does not have collision (EXCEPTION: MARBLES)
		return false;
	return true;
}

// Move Actor in the current direction
void Actor::moveInCurrDir()
{
	int dir = getDirection();
	double nx = getX();
	double ny = getY();

	switch (dir)
	{
	case right: nx++; break;
	case left: nx--; break;
	case up: ny++; break;
	case down: ny--; break;
	}

	moveTo(nx, ny);
}

// Checks if object is pushable in a direction
bool Actor::isPushableInDir(int dir)
{
	if (!isPushable()) // Actor is not a marble
		return false;

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
	entry = getWorld()->findEntryAtPos(static_cast<int>(x), static_cast<int>(y)); // find the entry at the position adjacent to the marble in direction dir

	if (entry == nullptr || entry->canSwallow())
	{
		moveTo(x, y);
		return true;
	}
	return false;
}

// IN-GAME ACCESSOR FUNCTIONS
// Gets the current HP of the actor
int Actor::getHP() const
{
	return m_hp;
}

// Gets world
StudentWorld* Actor::getWorld() const
{
	return m_world;
}

// IN-GAME MUTATOR FUNCTIONS
// Changes the HP of the actor to nhp
void Actor::changeHP(int nhp)
{
	m_hp = nhp;
}

// Changes actor to stolen (stolen by a thiefbot)
void Actor::changeStolen(bool s)
{
	stolen = s;
}

// IN-GAME STATUSFUNCTIONS
// Checks if the actor is alive
bool Actor::isAlive()
{
	return (getHP() > 0);
}

// Checks if actor (Collectible) is stolen by a thiefbot
bool Actor::isStolen()
{
	return stolen;
}

// INITIAL TYPE FUNCTIONS
// Checks if the actor has collision (cannot be walked into)
bool Actor::hasCollision()
{
	return collision;
}

// Checks if the actor can be shot (peas stop on the actor)
bool Actor::canBeShot()
{
	return shot;
}

// Checks if object (Collectible) can be taken 
bool Actor::canBeTaken()
{
	return taken;
}

// Checks if the object (ThiefBot) is part of the ThiefBotFactory census
bool Actor::isPartOfFactoryCensus()
{
	return factoryCensus;
}

// Checks if the object (Marble) is pushable
bool Actor::isPushable()
{
	return pushable;
}

// Checks if the object (Pit) can swallow a Marble
bool Actor::canSwallow()
{
	return swallow;
}

// INITIAL TYPE MUTATOR FUNCTIONS
// Changes Collision status
void Actor::changeCollision(bool c)
{
	collision = c;
}

// Changes canBeShot status
void Actor::changeCanBeShot(bool s)
{
	shot = s;
}

// Changes FactoryCensus status (Thiefbots)
void Actor::changeFactoryCensus(bool fc)
{
	factoryCensus = fc;
}

// Changes Pushable status (Marble)
void Actor::changePushable(bool p)
{
	pushable = p;
}

// Changes Swallow status (Pits)
void Actor::changeSwallow(bool s)
{
	swallow = s;
}

// Changes CanBeTaken status (Thiefbots)
void Actor::changeCanBeTaken(bool t)
{
	taken = t;
}

// PLAYER IMPLEMENTATIONS
Player::Player(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PLAYER, x, y, right, 20, sWorld), m_peas(20)
{}

// Player Action
// World reads an input from the user, the Player attempts to move in that direction
void Player::doSomething()
{
	if (!isAlive()) // player is dead
		return;

	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT: // Go in any direction
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
		case KEY_PRESS_SPACE: // Fire Pea
			if (m_peas > 0)
			{
				int dir = getDirection();
				double x = getX();
				double y = getY();

				Pea* np = new Pea(static_cast<int>(x), static_cast<int>(y), dir, getWorld());
				getWorld()->addToActors(np);
				m_peas--;
			}
			break;
		case KEY_PRESS_ENTER:
			setDirection(right);
			break;
		case KEY_PRESS_ESCAPE: // End level
			changeHP(0);
			break;
		}
	}
}

// Player doSomething Helper Function
// Determine if the player can move in the direction
bool Player::canMove(double x, double y)
{
	if (getWorld()->posHasActorWithCollision(x,y)) // pos has actor with collision
	{
		Actor* entry = getWorld()->getMarbleAtPos(x, y);
		if (entry != nullptr && entry->isPushableInDir(getDirection())) // check if it is a marble and can be pushed in a specified direction
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

// Get the number of peas the Player has
int Player::getPeas() const
{
	return m_peas;
}

// Give the players more peas
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
{
	changePushable(true); // Marbles can be pushed
}

// PIT IMPLEMENTATIONS
Pit::Pit(int x, int y, StudentWorld* sWorld)
	: Actor(IID_PIT, x, y, none, UNDAMAGEABLE, sWorld)
{
	changeCanBeShot(false); // Pits cannot be shot
	changeSwallow(true); // Pits can swallow Marbles
}

// Pit Action
// If a marble is on top of the Pit, the marble and pit will disappear
void Pit::doSomething()
{
	if (isAlive())
	{
		Actor* pm = getWorld()->getMarbleAtPos(getX(), getY());
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
	changeCanBeShot(false); // Peas cannot be shot
}

// Pea Action
// Move pea in the direction it faces until it makes contact with an Actor that can be shot
void Pea::doSomething()
{
	if (isAlive())
	{
		Actor* entry = nullptr;
		if (!moveInDirection(entry)) // moveInDirection loops until entry that can be shot is hit
		{
			entry->damage(); // damage the actor
			changeHP(0); // remove pea
		}
	}
}

// Pea doSomething() Helper Function
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


// THIEFBOTFACTORY IMPLEMENTATION
ThiefBotFactory::ThiefBotFactory(int x, int y, bool mean, StudentWorld* sWorld)
	: Actor(IID_ROBOT_FACTORY, x, y, none, UNDAMAGEABLE, sWorld), outputMean(mean)
{}

// ThiefBotFactory Action
// If census is true ( < 3 ThiefBots in 7x7 square), 1/50 chance to create a new thiefbot
void ThiefBotFactory::doSomething()
{
	if (census())
	{
		if (randInt(1, 50) == 1) // 1/50 chance
		{
			getWorld()->playSound(SOUND_ROBOT_BORN);
			if (!outputMean)
			{
				RegularThiefBot* nrtb = new RegularThiefBot(static_cast<int>(getX()), static_cast<int>(getY()), getWorld());
				getWorld()->addToActors(nrtb);
			}
			else
			{
				MeanThiefBot* nmtb = new MeanThiefBot(static_cast<int>(getX()), static_cast<int>(getY()), getWorld());
				getWorld()->addToActors(nmtb);
			}
		}
	}
}

// ThiefBotFactory doSomething() Helper Function
bool ThiefBotFactory::census()
{
	double centerX = getX();
	double centerY = getY();
	Actor* entry = getWorld()->getThiefBotAtPos(centerX, centerY); // Check if a thief bot is on the factory
	if (entry != nullptr)
		return false;

	int numThiefBots = getWorld()->countCensusInArea(centerX, centerY); // count the number of thiefbots in the 7x7 area
	return (numThiefBots < 3); // return false if there are 3 or more thief bots in the area
}

// ThiefBotFactory damage
// Redirects damage to a thiefbot on top of the factory
void ThiefBotFactory::damage()
{
	Actor* thiefonfac = getWorld()->getThiefBotAtPos(getX(), getY());
	if (thiefonfac != nullptr)
		thiefonfac->damage();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COLLECTIBLE: CRYSTAL, EXTRALIFEGOODIE, RESTOREHEALTHGOODIE, AMMOGOODIE, EXIT
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// COLLECTIBLE IMPLEMENTATIONS
Collectible::Collectible(int ID, int x, int y, int points, StudentWorld* sWorld)
	: Actor(ID, x, y, none, UNDAMAGEABLE, sWorld), m_points(points), stolen(false)
{
	changeCollision(false); // has no collision
	changeCanBeShot(false); // cannot be shot
	changeCanBeTaken(true); // can be picked up by player or thiefbot
}

// Collectible Actions
void Collectible::doSomething()
{
	if (isAlive())
	{
		if (getWorld()->isPlayerAt(getX(), getY()) && !isStolen()) // checks if player is on the collectible
		{
			getWorld()->increaseScore(m_points); // gives the player points
			getWorld()->playSound(SOUND_GOT_GOODIE); // plays sound
			doActivity(); // does action specific to the type of collectible
			changeHP(0); // remove collectible
		}
	}
}

// CRYSTAL IMPLEMENTATION
Crystal::Crystal(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_CRYSTAL, x, y, CRYSTAL_POINTS, sWorld)
{
	changeCanBeTaken(false); // cannot be stolen by a thiefbot
}

// Crystal Action
void Crystal::doActivity()
{
	getWorld()->decreaseNumCrystals(); // decrease the total number of crystals in the world
}

// EXTRALIFEGOODIE IMPLEMENTATION
ExtraLifeGoodie::ExtraLifeGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_EXTRA_LIFE, x, y, EXTRALIFE_POINTS, sWorld)
{}

// ExtraLifeGoodie Action
void ExtraLifeGoodie::doActivity()
{
	getWorld()->incLives(); // increase the lives of the player
}

// RESTOREHEALTHGOODIE IMPLEMENTATION
RestoreHealthGoodie::RestoreHealthGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_RESTORE_HEALTH, x, y, RESTOREHEALTH_POINTS, sWorld)
{}

// RestoreHealthGoodie Action
void RestoreHealthGoodie::doActivity()
{
	getWorld()->getPlayer()->changeHP(20); // set player's health back to 20
}

// AMMOGOODIE IMPLEMENTATION
AmmoGoodie::AmmoGoodie(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_AMMO, x, y, AMMO_POINTS, sWorld)
{}

// AmmoGoodie Action
void AmmoGoodie::doActivity()
{
	getWorld()->getPlayer()->addPeas(20); // give player 20 peas
}

// EXIT IMPLEMENTATION
Exit::Exit(int x, int y, StudentWorld* sWorld)
	: Collectible(IID_EXIT, x, y, FINISH_POINTS, sWorld), revealed(false)
{
	setVisible(false); // not visible until all crystals are collected
	changeCanBeTaken(false); // cannot be stolen by thief bots
}

// Exit Action
void Exit::doSomething()
{
	if (isAlive())
	{
		if (!revealed && getWorld()->noMoreCrystals()) // if it is not revealed and if the world has no more crystals
		{
			revealed = true; // reveal the crystal
			setVisible(true); // player can now see exit
			getWorld()->playSound(SOUND_REVEAL_EXIT); // play sound
		}

		if (revealed) // if revealed
		{
			if (getWorld()->isPlayerAt(getX(), getY()))  // check if the player is on the exit
			{
				getWorld()->increaseScore(FINISH_POINTS + getWorld()->getBonus()); // increase the score by points (completed level points + bonus)
				getWorld()->playSound(SOUND_FINISHED_LEVEL); // play sound
				getWorld()->completeLevel(); // notify world that the level is complete
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
	if (getWorld()->posCanBeTaken(getX(), getY(), collectible) && !collectible->isStolen()) // gets the item from the maze
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


	int i = randInt(0, 3);
	int startingDir = i * 90; // saves the original starting int if ThiefBot is completely obstructed

	std::vector<int> unvisitedDirs = { right, up, left, down }; // holds directions already checked

	while (!unvisitedDirs.empty())
	{
		if (canMoveInDir(unvisitedDirs[i]))
		{
			setDirection(unvisitedDirs[i]);
			return;
		}
		else
		{
			unvisitedDirs.erase(std::find(unvisitedDirs.begin(), unvisitedDirs.end(), unvisitedDirs[i]));
			i = randInt(0, unvisitedDirs.size() - 1);
		}
	}

	if (unvisitedDirs.empty())
		setDirection(startingDir);
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
			stolenItem->changeStolen(false);
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



