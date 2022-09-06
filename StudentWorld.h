#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class Peach;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  Actor* canMoveHere(int x, int y);
  Actor* isEmptyUnderneath();
  Actor* isEmptyUnderneath(Actor* actor);
  Actor* isEmptyUnderneathEnemy(Actor* actor, int f_or_b);
  //bool isPartlyEmptyUnderneath(int x, int y);
  Actor* isEmptyAbove();
  Peach* getPeach() { return peach; }
  ~StudentWorld();
  bool isTouchingPeach(int x, int y);
  Actor* isTouchingActor(int x, int y);
  void complete() { m_levelCompleted = true; }
  void win() { m_gameWon = true; }
  void add_to_actors(Actor* a) { actors.push_back(a); }
  string gameStatText;
private:
	std::vector<Actor*> actors;
	Peach* peach = nullptr;
	bool m_levelCompleted = false;
	bool m_gameWon = false;
};

#endif // STUDENTWORLD_H_
