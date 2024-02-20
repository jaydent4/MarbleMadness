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
: GameWorld(assetPath), m_numActors(0), m_bonus(1000)
{
}

StudentWorld::~StudentWorld()
{
    delete avatar;
    for (int i = 0; i < m_numActors; i++)
    {
        delete actors[i];
    }
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

    for (int xcoord = 0; xcoord < VIEW_WIDTH; xcoord++)
    {
            for (int ycoord = 0; ycoord < VIEW_WIDTH; ycoord++)
            {
                Level::MazeEntry atr = lev.getContentsOf(xcoord, ycoord);
                if (atr == Level::player)
                {
                    avatar = new Player(xcoord, ycoord, this);
                }
                else if (atr == Level::wall)
                {
                    Wall* nw = new Wall(xcoord, ycoord, this);
                    actors.push_back(nw);
                    m_numActors++;
                }
                // ADD OTHER ACTORS
            }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // UPDATE GAME STATUS LINE
    setGameStatText("Game will end when you type q");
    if (avatar->getHP() <= 0)
        return GWSTATUS_PLAYER_DIED;

    avatar->doSomething();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}

// HELPER FUNCTIONS
void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = m_bonus;
    int hpLeft = getLives();


}

void StudentWorld::getStatus(int score, int lev, int lives, int hp, int ammo, int bonus)
{
    ostringstream oss;

}


