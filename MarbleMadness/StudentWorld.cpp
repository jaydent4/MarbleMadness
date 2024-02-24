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
: GameWorld(assetPath), m_bonus(1000)
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    int isLevelLoaded = loadCurrentLevel(lev);
    if (isLevelLoaded == -1)
        return GWSTATUS_LEVEL_ERROR;
    if (isLevelLoaded == 0 || getLevel() == 99)
        return GWSTATUS_PLAYER_WON;
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

        /*
        if completed level, return completed level
        */
        
    }
    removeDeadEntries();
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

    // checks if level 
    if (resultlvl == Level::load_fail_bad_format)
        return -1;
    else if (resultlvl == Level::load_fail_file_not_found)
        return 0;
    return 1;
}

Marble* StudentWorld::getMarbleAtPos(double x, double y)
{
    for (list<Actor*>::iterator p = actors.begin(); p != actors.end(); p++)
    {
        if ((*p)->getX() == x && (*p)->getY() == y && dynamic_cast<Marble*>(*p) != nullptr)
        {
            return dynamic_cast<Marble*>(*p);
        }
    }
    return nullptr;
}

bool StudentWorld::isEntryAlive(Actor* entry)
{
    return entry->isAlive();
}

void StudentWorld::removeDeadEntries()
{
    list<Actor*>::iterator p = actors.begin();
    while (p != actors.end())
    {
        if (!isEntryAlive(*p))
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