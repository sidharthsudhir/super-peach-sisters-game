#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <random>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    Level lev(assetPath());
    //std::vector<Actor*> actors;
    string level_file;
    if (getLevel() < 10)
    {
        level_file = "level0" + to_string(getLevel()) + ".txt";
    }
    else
    {
        level_file = "level" + to_string(getLevel()) + ".txt";
    }
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                Level::GridEntry ge;
                ge = lev.getContentsOf(i, j);
                switch (ge)
                {
                    case Level::empty:
                    {
                        break;
                    }
                    ////////////////////////////////////////////
                    case Level::peach:
                    {
                        peach = new Peach(i, j, this);
                        peach->makePassable();
                        break;
                    }
                    ////////////////////////////////////////////
                    case Level::block:
                    {
                        Block* b = new Block(i, j, this, false, nullptr);
                        b->makeNotPassable();
                        actors.push_back(b);
                        break;
                    }
                    case Level::flower_goodie_block:
                    {
                        Flower* f = new Flower(i, j + 1, this);
                        f->setVisible(false);
                        f->makePassable();
                        actors.push_back(f);
                        Block* b = new Block(i, j, this, true, f);
                        b->makeNotPassable();
                        actors.push_back(b);
                        break;
                    }
                    case Level::mushroom_goodie_block:
                    {
                        Mushroom* m = new Mushroom(i, j + 1, this);
                        m->setVisible(false);
                        m->makePassable();
                        actors.push_back(m);
                        Block* b = new Block(i, j, this, true, m);
                        b->makeNotPassable();
                        actors.push_back(b);
                        break;
                    }
                    case Level::star_goodie_block:
                    {
                        Star* s = new Star(i, j + 1, this);
                        s->setVisible(false);
                        s->makePassable();
                        actors.push_back(s);
                        Block* b = new Block(i, j, this, true, s);
                        b->makeNotPassable();
                        actors.push_back(b);
                        break;
                    }
                    case Level::pipe:
                    {
                        Pipe* p = new Pipe(i, j, this);
                        p->makeNotPassable();
                        actors.push_back(p);
                        break;
                    }
                    ////////////////////////////////////////////
                    case Level::flag:
                    {
                        Flag* f = new Flag(i, j, this);
                        f->makePassable();
                        actors.push_back(f);
                        break;
                    }
                    case Level::mario:
                    {
                        Mario* m = new Mario(i, j, this);
                        m->makePassable();
                        actors.push_back(m);
                        break;
                    }
                    ////////////////////////////////////////////
                    case Level::goomba:
                    {
                        int direction = (rand() % 2) * 180;
                        Goomba* g = new Goomba(i, j, this, direction);
                        g->makePassable();
                        actors.push_back(g);
                        break;
                    }
                    case Level::koopa:
                    {
                        int direction = (rand() % 2) * 180;
                        Koopa* k = new Koopa(i, j, this, direction);
                        k->makePassable();
                        actors.push_back(k);
                        break;
                    }
                    case Level::piranha:
                    {
                        int direction = (rand() % 2) * 180;
                        Piranha* p = new Piranha(i, j, this, direction);
                        p->makePassable();
                        actors.push_back(p);
                        break;
                    }
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (peach->remainingTempInvincibilityTicks() != 0)
        peach->reduceTempInvincibilityTicks();
    gameStatText = "Lives: " + to_string(getLives())
        + "  Level: " + to_string(getLevel())
        + "  Points: " + to_string(getScore());

    if (peach->isInvincible()) gameStatText = gameStatText + " StarPower!";
    if (peach->hasJumpPower()) gameStatText = gameStatText + " JumpPower!";
    if (peach->hasShootPower()) gameStatText = gameStatText + " ShootPower!";

    if (m_levelCompleted)
    {
        playSound(SOUND_FINISHED_LEVEL);
        m_levelCompleted = false;
        peach->setAlive();
        cleanUp();
        return GWSTATUS_FINISHED_LEVEL;
    }

    if (m_gameWon)
    {
        playSound(SOUND_GAME_OVER);
        peach->setAlive();
        cleanUp();
        return GWSTATUS_PLAYER_WON;
    }

    if (peach->isDead())
    {
        playSound(SOUND_PLAYER_DIE);
        peach->setAlive();
        decLives();
        cleanUp();
        return GWSTATUS_PLAYER_DIED;
    }

    peach->doSomething();
    for (int i = 0; i < actors.size(); i++)
    {
        actors[i]->doSomething();
        if (actors[i]->isDead())
        {
            delete actors[i];
            actors.erase(actors.begin() + i);
        }

    }
    setGameStatText(gameStatText);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;

    for (it = actors.begin(); it < actors.end(); it++)
    {
        delete* it;
    }

    actors.clear();
    if (peach != nullptr)
    {
        delete peach;
        peach = nullptr;
    }

}

StudentWorld::~StudentWorld()
{
     cleanUp();
}

Actor* StudentWorld::canMoveHere(int x, int y)
{
    for (int i = 0; i < actors.size(); i++)
    {
        Actor* a = actors[i];
        if (
            !a->isPassable()
            && (
                (x > a->getX() && x < a->getX() + SPRITE_WIDTH)
                || (x + SPRITE_WIDTH > a->getX() && x + SPRITE_WIDTH < a->getX() + SPRITE_WIDTH)
                )
            && (
                (y >= a->getY() && y < a->getY() + SPRITE_HEIGHT)
                || (y + SPRITE_HEIGHT > a->getY() && y + SPRITE_HEIGHT < a->getY() + SPRITE_HEIGHT)
                )
            )
        {
            return a;
        }
    }
    return nullptr;
}

Actor* StudentWorld::isEmptyUnderneath()
{
    for (int i = 0; i < actors.size(); i++)
    {
        Actor* a = actors[i];
        if (
            !a->isPassable()
            && (
                (peach->getX() > a->getX() && peach->getX() < a->getX() + SPRITE_WIDTH)
                || (peach->getX() + SPRITE_WIDTH > a->getX() && peach->getX() + SPRITE_WIDTH <= a->getX() + SPRITE_WIDTH)
            )
            && peach->getY() == a->getY() + SPRITE_HEIGHT)
        {
            return a;
        }
    }
    return nullptr;
}

Actor* StudentWorld::isEmptyUnderneath(Actor* actor)
{
    for (int i = 0; i < actors.size(); i++)
    {
        Actor* a = actors[i];
        if (
            !a->isPassable()
            &&(
                (actor->getX() > a->getX() && actor->getX() < a->getX() + SPRITE_WIDTH)
                || (actor->getX() + SPRITE_WIDTH > a->getX() && actor->getX() + SPRITE_WIDTH <= a->getX() + SPRITE_WIDTH)
                )
            && actor->getY() == a->getY() + SPRITE_HEIGHT)
        {
            return a;
        }
    }
    return nullptr;
}

Actor* StudentWorld::isEmptyUnderneathEnemy(Actor* actor, int f_or_b)
{
    for (int i = 0; i < actors.size(); i++)
    {
        Actor* a = actors[i];
        if (
            !a->isPassable()
            && (
                (actor->getX() + f_or_b > a->getX() && actor->getX() + f_or_b < a->getX() + SPRITE_WIDTH)
                || (actor->getX() + f_or_b + SPRITE_WIDTH > a->getX() && actor->getX() + f_or_b + SPRITE_WIDTH <= a->getX() + SPRITE_WIDTH)
                )
            && actor->getY() == a->getY() + SPRITE_HEIGHT)
        {
            return a;
        }
    }
    return nullptr;
}


Actor* StudentWorld::isEmptyAbove()
{
    for (int i = 0; i < actors.size(); i++)
    {
        Actor* a = actors[i];
        if (
            !a->isPassable()
            && (
                (peach->getX() > a->getX() && peach->getX() < a->getX() + SPRITE_WIDTH)
                || (peach->getX() + SPRITE_WIDTH > a->getX() && peach->getX() + SPRITE_WIDTH <= a->getX() + SPRITE_WIDTH)
                )
            && peach->getY() + SPRITE_HEIGHT == a->getY()
            )
        {
            return a;
        }
    }
    return nullptr;
}

bool StudentWorld::isTouchingPeach(int x, int y)
{
    if (
        (
            (peach->getX() >= x && peach->getX() < x + SPRITE_WIDTH)
        || (peach->getX() + SPRITE_WIDTH > x && peach->getX() + SPRITE_WIDTH < x + SPRITE_WIDTH)
            )
        && (
            (peach->getY() >= y && peach->getY() < y + SPRITE_HEIGHT)
        ||  (peach->getY() + SPRITE_HEIGHT > y && peach->getY() + SPRITE_HEIGHT < y + SPRITE_HEIGHT)
            )
        )
    {
        return true;
    }
    return false;
}

Actor* StudentWorld::isTouchingActor(int x, int y)
{
    for (int i = 0; i < actors.size(); i++)
    {
        Actor* a = actors[i];
        if (
            a->isPassable()
            && (
                (x > a->getX() && x < a->getX() + SPRITE_WIDTH)
                || (x + SPRITE_WIDTH > a->getX() && x + SPRITE_WIDTH < a->getX() + SPRITE_WIDTH)
                )
            && (
                (y >= a->getY() && y < a->getY() + SPRITE_HEIGHT)
                || (y + SPRITE_HEIGHT > a->getY() && y + SPRITE_HEIGHT < a->getY() + SPRITE_HEIGHT)
                )
            )
        {
            return a;
        }
    }
    return nullptr;
}