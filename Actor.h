#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"

class Actor : public GraphObject
{
public:
	Actor(int imageID, int x, int y, StudentWorld* sw, int direction, int depth, double size)
		: GraphObject(imageID, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, direction, depth, size), m_world(sw) {}
	bool isDead() const { return m_dead; }
	void setDead() { m_dead = true; }
	void setAlive() { m_dead = false; }
	bool isPassable() { return canPassThrough; }
	void makePassable() { canPassThrough = true; }
	void makeNotPassable() { canPassThrough = false; }
	virtual void doSomething() = 0;
	virtual void bonk() = 0;
	virtual void getDamaged() = 0;
	StudentWorld* getWorld() const { return m_world; }

private:
	bool m_dead = false;
	StudentWorld* m_world;	
	bool canPassThrough = false;
};

///////////////////////////////////

class Peach : public Actor
{
public:
	// Housekeeping func
	Peach(int x, int y, StudentWorld* sw)
		: Actor(IID_PEACH, x, y, sw, 0, 0, 1.0) {}
	void doSomething();
	void bonk();
	void getDamaged() { }

	// Accessors
	bool isInvincible() { return m_hasInvincibility; }
	bool hasJumpPower() { return m_hasJumpPower; }
	bool hasShootPower() { return m_hasShootPower; }
	int remainingTempInvincibilityTicks() { return m_remainingTempInvincibilityTicks; }
	int remainingHitPoints() { return hit_point; }

	//Mutators
	//void recharge();
	void makeInvincible() { m_hasInvincibility = true; m_remainingInvincibilityTicks = 150; };
	void makeTemporarilyInvincible() { m_remainingTempInvincibilityTicks = 10; }
	void reduceTempInvincibilityTicks() { m_remainingTempInvincibilityTicks--; }
	void giveJumpPower() { m_hasJumpPower = true; }
	void giveShootPower() { m_hasShootPower = true; }
	void removeJumpPower() { m_hasJumpPower = false; }
	void removeShootPower() { m_hasShootPower = false; }

	void decHitPoints() { hit_point--; }
	void incHitPoints() { hit_point++; }


private:
	bool m_hasJumpPower = false;
	bool m_hasShootPower = false;
	bool m_hasInvincibility = false;
	bool m_isRecharging = false;
	int m_jumpTime = 0;
	int m_ticksRemainingToRecharge = 0;
	int hit_point = 1;
	int m_remainingInvincibilityTicks = 0;
	int m_remainingTempInvincibilityTicks = 0;
};

///////////////////////////////////

class Goodie : public Actor
{
public:
	Goodie(int imageID, int x, int y, StudentWorld* sw)
		: Actor(imageID, x, y, sw, 0, 1, 1.0) {}
	virtual void doSomething() = 0;
	void bonk() {};
	void getDamaged() { }
	void makeReady() { ready = true; }
	void spend() { setVisible(false); ready = false; spent = true; }
	bool isReady() { return ready; }
	bool isSpent() { return spent; }
protected:
	void moevement();
private:
	bool ready = false;
	bool spent = false;
	Peach* m_peach;
};

class Flower : public Goodie
{
public:
	Flower(int x, int y, StudentWorld* sw)
		: Goodie(IID_FLOWER, x, y, sw) {}
	void doSomething();
};

class Mushroom : public Goodie
{
public:
	Mushroom(int x, int y, StudentWorld* sw)
		: Goodie(IID_MUSHROOM, x, y, sw) {}
	void doSomething();
};

class Star : public Goodie
{
public:
	Star(int x, int y, StudentWorld* sw)
		: Goodie(IID_STAR, x, y, sw) {}
	void doSomething();
};

///////////////////////////////////

class Ground : public Actor
{
public:
	Ground(int imgID, int x, int y, StudentWorld* sw, int direction, int depth, double size)
		: Actor(imgID, x, y, sw, direction, depth, size) {}
	virtual void doSomething() {};
	virtual void bonk() = 0;
	void getDamaged() { }
};

class Block : public Ground
{
public:
	Block(int x, int y, StudentWorld* sw, bool hasGoodie, Goodie* goodie)
		: Ground(IID_BLOCK, x, y, sw, 0, 2, 1.0), m_hasGoodie(hasGoodie), m_goodie(goodie) {}
	void bonk();
private:
	bool m_hasGoodie = 0;
	Goodie* m_goodie;
};

class Pipe : public Ground
{
public:
	Pipe(int x, int y, StudentWorld* sw)
		: Ground(IID_PIPE, x, y, sw, 0, 2, 1.0) {}
	void bonk();
};

///////////////////////////////////

class Goals : public Actor
{
public:
	Goals(int imgID, int x, int y, StudentWorld* sw)
		: Actor(imgID, x, y, sw, 0, 1, 1.0) {}
	virtual void doSomething() {};
	virtual void bonk() = 0;
	void getDamaged() { }
};

class Flag : public Goals
{
public: 
	Flag(int x, int y, StudentWorld* sw)
		: Goals(IID_FLAG, x, y, sw) {}
	void doSomething();
	void bonk() {}
};

class Mario : public Goals
{
public:
	Mario(int x, int y, StudentWorld* sw)
		: Goals(IID_MARIO, x, y, sw) {}
	void doSomething();
	void bonk() {}
};

///////////////////////////////////

class Enemy : public Actor
{
public:
	Enemy(int imageID, int x, int y, StudentWorld* sw, int direction)
		: Actor(imageID, x, y, sw, direction, 0, 1.0) {}
	virtual void doSomething() = 0;
	virtual void bonk() = 0;
	virtual void getDamaged() = 0;

};

class Goomba : public Enemy
{
public:
	Goomba(int x, int y, StudentWorld* sw, int direction)
		: Enemy(IID_GOOMBA, x, y, sw, direction){}
	void doSomething();
	void bonk();
	void getDamaged();
};

class Koopa : public Enemy
{
public:
	Koopa(int x, int y, StudentWorld* sw, int direction)
		: Enemy(IID_KOOPA, x, y, sw, direction) {}
	void doSomething();
	void bonk();
	void getDamaged();
};

class Piranha : public Enemy
{
public:
	Piranha(int x, int y, StudentWorld* sw, int direction)
		: Enemy(IID_PIRANHA, x, y, sw, direction) {}
	void doSomething();
	void bonk();
	void getDamaged();
private:
	int m_firingDelay = 0;
};

///////////////////////////////////

class Projectile : public Actor
{
public:
	Projectile(int imageID, int x, int y, StudentWorld* sw, int direction)
		: Actor(imageID, x, y, sw, direction, 1, 1.0) {}
	virtual void doSomething() = 0;
	virtual void bonk() = 0;
	void getDamaged() { }
};

class PiranhaFireball : public Projectile
{
public:
	PiranhaFireball(int x, int y, StudentWorld* sw, int direction)
		: Projectile(IID_PIRANHA_FIRE, x, y, sw, direction) {}
	void doSomething();
	void bonk() {};
};

class PeachFireball : public Projectile
{
public:
	PeachFireball(int x, int y, StudentWorld* sw, int direction)
		: Projectile(IID_PEACH_FIRE, x, y, sw, direction) {}
	void doSomething();
	void bonk() {};
};

class Shell : public Projectile
{
public:
	Shell(int x, int y, StudentWorld* sw, int direction)
		: Projectile(IID_SHELL, x, y, sw, direction) {}
	void doSomething();
	void bonk() {};
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif