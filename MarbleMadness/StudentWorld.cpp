#include "StudentWorld.h"
#include "GameConstants.h"
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

// PUBLIC
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_bonus(1000), m_numCrystals(0)
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
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
    Level lev(assetPath());
    Level::LoadResult resultlvl = lev.loadLevel(currlvl);

    // checks if level 
    if (resultlvl == Level::load_fail_file_not_found || resultlvl == Level::load_fail_bad_format)
        return -1;

    Level::MazeEntry atr;
    levelCompleted = false;
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
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // UPDATE GAME STATUS LINE
    setDisplayText();
    avatar->doSomething();
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        // calls all actors doSomething()
        (*p)->doSomething(); 
        
        // checks if player/avatar is alive
        if (!(avatar->isAlive()))
            return GWSTATUS_PLAYER_DIED;

        // check if the player completed
        if (levelCompleted)
            return GWSTATUS_FINISHED_LEVEL;
    }
    removeDeadEntries();

    decreaseBonus();

    if (levelCompleted)
        return GWSTATUS_FINISHED_LEVEL;

	return GWSTATUS_CONTINUE_GAME;
}

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

// HELPER FUNCTIONS
Actor* StudentWorld::findEntryAtPos(double x, double y)
{
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y)
            return (*p);
    }
    return nullptr;
}

void StudentWorld::addToActors(Actor* na)
{
    actors.push_back(na);
}

Player* StudentWorld::getPlayer() const
{
    return avatar;
}

void StudentWorld::decreaseNumCrystals()
{
    m_numCrystals--;
}

bool StudentWorld::noMoreCrystals()
{
    return (m_numCrystals <= 0);
}

int StudentWorld::getBonus() const
{
    return m_bonus;
}

void StudentWorld::completeLevel()
{
    levelCompleted = true;
}

void StudentWorld::decreaseBonus()
{
    if (m_bonus != 0)
    {
        m_bonus--;
    }
}

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



bool StudentWorld::isPlayerAt(double x, double y)
{
    return (avatar->getX() == x && avatar->getY() == y);
}

bool StudentWorld::isEntryAlive(Actor* entry)
{
    return entry->isAlive();
}

