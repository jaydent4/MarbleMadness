#include "StudentWorld.h"
#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

// CONSTRUCTOR
StudentWorld::StudentWorld(std::string assetPath)
    : GameWorld(assetPath), m_bonus(1000), m_numCrystals(0)
{}

// DESTRUCTOR
StudentWorld::~StudentWorld()
{
    cleanUp();
}

// GAME FUNCTIONALITY FUNCTIONS
// init(): load maze, initialize data structure, allocate avatar, allocate other actor objects
int StudentWorld::init()
{
    Level lev(assetPath());
    int isLevelLoaded = loadCurrentLevel(lev);
    if (isLevelLoaded == -1)
        return GWSTATUS_LEVEL_ERROR;
    if (isLevelLoaded == 0 || getLevel() == 99)
        return GWSTATUS_PLAYER_WON;

    levelCompleted = false;
    m_bonus = 1000;
    Level::MazeEntry atr;
    for (int xcoord = 0; xcoord < VIEW_WIDTH; xcoord++)
    {
        for (int ycoord = 0; ycoord < VIEW_WIDTH; ycoord++)
        {
            atr = lev.getContentsOf(xcoord, ycoord);
            switch (atr)
            {
            case Level::player:
            {
                avatar = new Player(xcoord, ycoord, this);
                break;
            }
            case Level::wall:
            {
                Wall* nw = new Wall(xcoord, ycoord, this);
                actors.push_back(nw);
                break;
            }
            case Level::marble:
            {
                Marble* nm = new Marble(xcoord, ycoord, this);
                actors.push_back(nm);
                break;
            }
            case Level::pit:
            {
                Pit* np = new Pit(xcoord, ycoord, this);
                actors.push_back(np);
                break;
            }
            case Level::crystal:
            {
                Crystal* nc = new Crystal(xcoord, ycoord, this);
                actors.push_back(nc);
                m_numCrystals++;
                break;
            }
            case Level::extra_life:
            {
                ExtraLifeGoodie* nelg = new ExtraLifeGoodie(xcoord, ycoord, this);
                actors.push_back(nelg);
                break;
            }
            case Level::restore_health:
            {
                RestoreHealthGoodie* nrhg = new RestoreHealthGoodie(xcoord, ycoord, this);
                actors.push_back(nrhg);
                break;
            }
            case Level::ammo:
            {
                AmmoGoodie* nag = new AmmoGoodie(xcoord, ycoord, this);
                actors.push_back(nag);
                break;
            }
            case Level::exit:
            {
                Exit* ne = new Exit(xcoord, ycoord, this);
                actors.push_back(ne);
                break;
            }
            case Level::horiz_ragebot:
            {
                RageBot* nhrb = new RageBot(xcoord, ycoord, RIGHT, this);
                actors.push_back(nhrb);
                break;
            }
            case Level::vert_ragebot:
            {
                RageBot* nvrb = new RageBot(xcoord, ycoord, UP, this);
                actors.push_back(nvrb);
                break;
            }
            case Level::thiefbot_factory:
            {
                ThiefBotFactory* ntbf = new ThiefBotFactory(xcoord, ycoord, false, this);
                actors.push_back(ntbf);
                break;
            }
            case Level::mean_thiefbot_factory:
            {
                ThiefBotFactory* nmtbf = new ThiefBotFactory(xcoord, ycoord, true, this);
                actors.push_back(nmtbf);
                break;
            }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

// move(): call avatar and each actor to doSomething()
// if avatar is dead, return specified value
// if the level is completed, return specified value
int StudentWorld::move()
{
    // UPDATE GAME STATUS LINE
    setDisplayText();
    avatar->doSomething();
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->isAlive())
        {
            // calls all actors doSomething()
            (*p)->doSomething();

            // checks if player/avatar is alive
            if (!(avatar->isAlive()))
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }

            // check if the player completed
            if (levelCompleted)
                return GWSTATUS_FINISHED_LEVEL;
        }
    }
    removeDeadEntries();

    decreaseBonus();

    if (levelCompleted)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}

// cleanUp(): deallocate avatar pointer and actor pointers, remove them from the list
void StudentWorld::cleanUp()
{
    // delete avatar
    if (avatar != nullptr)
    {
        delete avatar; // deletes avatar
        avatar = nullptr; // checks if avatar points to an object
    }
    // delete actors
    if (!actors.empty()) // checks if list is empty (no actors)
    {
        list<Actor*>::iterator p = actors.begin();
        while (p != actors.end())
        {
            delete (*p);
            p = actors.erase(p);
        }
    }
}

// STATUS FUNCTIONS
// Determines if there are no more crystals in the level
bool StudentWorld::noMoreCrystals()
{
    return (m_numCrystals <= 0);
}

// Determines if player is at (x, y)
bool StudentWorld::isPlayerAt(double x, double y)
{
    return (avatar->getX() == x && avatar->getY() == y);
}

// Determines if player is in line of sight
bool StudentWorld::clearShotToPlayerExists(double ix, double iy, int dir)
{
    double dx = 0;
    double dy = 0;

    switch (dir)
    {
    case RIGHT: dx++; break;
    case LEFT: dx--; break;
    case UP: dy++; break;
    case DOWN: dy--; break;
    }

    Actor* dummy; // irrelevant actor
    while ((findEntryAtPos(ix + dx, iy + dy) == nullptr || !posStopsPeas(ix + dx, iy + dy, dummy)) && !isPlayerAt(ix + dx, iy + dy))
    {

        switch (dir)
        {
        case RIGHT: dx++; break;
        case LEFT: dx--; break;
        case UP: dy++; break;
        case DOWN: dy--; break;
        }
    }
    return isPlayerAt(ix + dx, iy + dy); // check if player is at the position the line of sight stopped at
}

// ACCESSOR FUNCTIONS
// Finds the entry at (x, y)
Actor* StudentWorld::findEntryAtPos(double x, double y)
{
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y)
            return (*p); // does not work for cases where there are more than one Actor on a unit
    }
    return nullptr;
}

// Get the ThiefBot at position (x, y)
Actor* StudentWorld::getThiefBotAtPos(double x, double y)
{
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y && (*p)->isPartOfFactoryCensus()) // Only thief bots are part of factory census
        {
            return (*p);
        }
    }
    return nullptr;
}

// Gets marble at (x, y)
Actor* StudentWorld::getMarbleAtPos(double x, double y)
{
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y && (*p)->isPushable())
        {
            return (*p);
        }
    }
    return nullptr;
}

// Gets avatar
Player* StudentWorld::getPlayer() const
{
    return avatar;
}

// Determines whether a position will stop a pea (i.e. the position->canBeShot() == true)
bool StudentWorld::posStopsPeas(double x, double y, Actor*& entry)
{
    if (isPlayerAt(x, y))
        return true;
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y)
        {
            if ((*p)->canBeShot())
            {
                entry = *p;
                return true;
            }
        }
    }
    return false;
}

// Determines whether a position has an Actor with collision
bool StudentWorld::posHasActorWithCollision(double x, double y)
{
    if (isPlayerAt(x, y))
        return true;

    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y)
        {
            if ((*p)->hasCollision())
                return true;
        }
    }
    return false;
}

// Determines whether the Actor (Collectible) at (x, y) can be taken, provides the collectible
bool StudentWorld::posCanBeTaken(double x, double y, Actor*& collectible)
{
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y)
        {
            if ((*p)->canBeTaken())
            {
                collectible = *p;
                return true;
            }
        }
    }
    return false;
}

// Counts the number of thief bots in a 7x7 area
int StudentWorld::countCensusInArea(double x, double y)
{
    int numThiefBots = 0;
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->isPartOfFactoryCensus() && (*p)->getX() >= x - 3 && (*p)->getX() <= x + 3 && (*p)->getY() >= y - 3 && (*p)->getY() <= y + 3)
            numThiefBots++;
        if (numThiefBots >= 3)
            break;
    }
    return numThiefBots;
}

// Gets bonus from world
int StudentWorld::getBonus() const
{
    return m_bonus;
}

// MUTATOR FUNCTIONS
// Add actor to actors list
void StudentWorld::addToActors(Actor* na)
{
    actors.push_back(na);
}

// Decrease the number of crystals in the world
void StudentWorld::decreaseNumCrystals()
{
    m_numCrystals--;
}

// Decrease the bonus by 1
void StudentWorld::decreaseBonus()
{
    if (m_bonus != 0)
    {
        m_bonus--;
    }
}

// Complete the level
void StudentWorld::completeLevel()
{
    levelCompleted = true;
}

// HELPER FUNCTIONS
// Display the score line text
void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = m_bonus;
    int lives = getLives();
    int hp = avatar->getHP();
    int ammo = avatar->getPeas();

    string status = getStatus(score, level, lives, hp, ammo, bonus);
    setGameStatText(status);
}

// Obtains all values and converts it into a full string
std::string StudentWorld::getStatus(int score, int lev, int lives, int hp, int ammo, int bonus)
{

    std::string status = "";

    for (int i = 1; i < 7; i++)
    {
        ostringstream stat;
        switch (i)
        {
        case 1: // score
            stat.fill('0');
            stat << setw(7) << score;
            status += ("Score: " + stat.str() + "  ");
            break;
        case 2: // level
            stat.fill('0');
            stat << setw(2) << lev;
            status += ("Level: " + stat.str() + "  ");
            break;
        case 3: // lives
            stat.fill(' ');
            stat << setw(2) << lives;
            status += ("Lives: " + stat.str() + "  ");
            break;
        case 4: // health
            stat.fill(' ');
            stat << setw(3) << hp * 5 << "%";
            status += ("Health: " + stat.str() + "  ");
            break;
        case 5: // ammo
            stat.fill(' ');
            stat << setw(3) << ammo;
            status += ("Ammo: " + stat.str() + "  ");
            break;
        case 6: // bonus
            stat.fill(' ');
            stat << setw(4) << bonus;
            status += ("Bonus: " + stat.str());
            break;
        }
    }
    return status;
}

// loads the current level, returns specified values depending on if the file is valid, invalid, or cannot be found
int StudentWorld::loadCurrentLevel(Level& lev)
{
    // GETTING LEVEL
    // gets level file name
    int lvl = getLevel();
    string currlvl = "level";
    if (lvl < 10)
        currlvl += "0";
    currlvl += (lvl + 48);
    currlvl += ".txt";

    // loads level
    Level::LoadResult resultlvl = lev.loadLevel(currlvl);
    // Level::LoadResult resultlvl = lev.loadLevel("TEST.txt");

    // checks if level 
    if (resultlvl == Level::load_fail_bad_format)
        return -1;
    else if (resultlvl == Level::load_fail_file_not_found)
        return 0;
    return 1;
}

// Finds all actors that are not alive (hp <= 0) and removing them from the list
void StudentWorld::removeDeadEntries()
{
    list<Actor*>::iterator p = actors.begin();
    while (p != actors.end())
    {
        if (!((*p)->isAlive()))
        {
            delete (*p);
            p = actors.erase(p);
        }
        else
        {
            p++;
        }
    }
}