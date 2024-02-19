#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp


// PUBLIC
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    vector<Actor*> actors;
    Player* avatar;

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
}

// PRIVATE
bool StudentWorld::loadLvl()
{
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

}


