#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <iostream>
using namespace std;

////////////////////////////////////////
// PEACH FUNCTIONS
////////////////////////////////////////

void Peach::doSomething()
{
	if (isDead())
		return;
	if (m_ticksRemainingToRecharge != 0)
	{
		m_ticksRemainingToRecharge--;
	}
	if (isInvincible())
	{
		m_remainingInvincibilityTicks--;
		if (m_remainingInvincibilityTicks == 0) m_hasInvincibility = false;
		Actor* a = getWorld()->isTouchingActor(getX(), getY());
		if (a != nullptr) a->bonk();
	}
	if (getWorld()->isEmptyUnderneath() == nullptr && m_jumpTime == 0)
	{
		moveTo(getX(), getY() - 4);
	}
	if (m_jumpTime != 0)
	{
		if (getWorld()->isEmptyAbove() == nullptr)
		{
			moveTo(getX(), getY() + 4);
			m_jumpTime--;
		}
		else
		{
			getWorld()->isEmptyAbove()->bonk();
			m_jumpTime = 0;
		}
	}
	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
			case KEY_PRESS_LEFT:
			{
				if (getWorld()->canMoveHere((getX() - 4), getY()) == nullptr)
				{
					moveTo(getX() - 4, getY());
					setDirection(180);
				}
				else
				{
					getWorld()->canMoveHere((getX() - 4), getY())->bonk();
				}
				break;
			}
			case KEY_PRESS_RIGHT:
			{
				if (getWorld()->canMoveHere((getX() + 4), getY()) == nullptr)
				{
					moveTo(getX() + 4, getY());
					setDirection(0);
				}
				else
				{
					getWorld()->canMoveHere((getX() + 4), getY())->bonk();
				}
				break;
			}
			case KEY_PRESS_SPACE:
			{
				if (hasShootPower() && m_ticksRemainingToRecharge == 0)
				{
					int to_add;
					if (getDirection() == 0) to_add = 1;
					if (getDirection() == 180) to_add = 0;
					PeachFireball* pf = new PeachFireball((getX() / SPRITE_WIDTH) + to_add, getY() / SPRITE_HEIGHT, getWorld(), getDirection());
					pf->makePassable();
					getWorld()->add_to_actors(pf);
					m_ticksRemainingToRecharge = 8;
					getWorld()->playSound(SOUND_PLAYER_FIRE);
				}
				break;
			}
			case KEY_PRESS_UP:
			{
				if (m_jumpTime == 0 && getWorld()->isEmptyUnderneath() != nullptr)
				{
					getWorld()->playSound(SOUND_PLAYER_JUMP);
					if (m_hasJumpPower == true)
					{
						m_jumpTime = 12;
					}
					else
					{
						m_jumpTime = 8;
					}
				}
			}
		}
	}
}

void Peach::bonk()
{
	if (isInvincible() || remainingTempInvincibilityTicks() != 0)
		return;
	getWorld()->getPeach()->decHitPoints();
	makeTemporarilyInvincible();
	getWorld()->getPeach()->removeJumpPower();
	getWorld()->getPeach()->removeShootPower();
	if (getWorld()->getPeach()->remainingHitPoints() > 0)
		getWorld()->playSound(SOUND_PLAYER_HURT);
	if (getWorld()->getPeach()->remainingHitPoints() <= 0)
		getWorld()->getPeach()->setDead();
}

////////////////////////////////////////
// BLOCK FUNCTIONS
////////////////////////////////////////

void Block::bonk()
{
	if (isDead())
		return;
	if (m_hasGoodie == false)
	{
		getWorld()->playSound(SOUND_PLAYER_BONK);
		return;
	}
	else if (m_hasGoodie == true && !m_goodie->isSpent())
	{
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		m_goodie->setVisible(true);
		m_goodie->makeReady();
	}
}

////////////////////////////////////////
// PIPE FUNCTIONS
////////////////////////////////////////

void Pipe::bonk()
{
		
}

////////////////////////////////////////
// FLAG FUNCTIONS
////////////////////////////////////////

void Flag::doSomething()
{
	if (isDead())
		return;
	if (getWorld()->isTouchingPeach(getX(), getY()))
	{
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->complete();
	}
}

////////////////////////////////////////
// MARIO FUNCTIONS
////////////////////////////////////////

void Mario::doSomething()
{
	if (isDead())
		return;
	if (getWorld()->isTouchingPeach(getX(), getY()))
	{
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->win();
	}
}

////////////////////////////////////////
// GOODIE FUNCTIONS
////////////////////////////////////////

void Goodie::moevement()
{
	if (getWorld()->isEmptyUnderneath(this) == nullptr)
	{
		moveTo(getX(), getY() - 2);
	}
	if (getDirection() == 0)
	{
		if (getWorld()->canMoveHere(getX() + 2, getY()) == nullptr)
		{
			moveTo(getX() + 2, getY());
		}
		else
		{
			setDirection(180);
		}
	}
	else if (getDirection() == 180)
	{
		if (getWorld()->canMoveHere(getX() - 2, getY()) == nullptr)
		{
			moveTo(getX() - 2, getY());
		}
		else
		{
			setDirection(0);
		}
	}
	
}

////////////////////////////////////////
// FLOWER FUNCTIONS
////////////////////////////////////////

void Flower::doSomething()
{
	if (isDead())
		return;
	if (isReady())
	{
		this->moevement();
	}
	if (getWorld()->isTouchingPeach(getX(), getY()) && isReady())
	{
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		setDead();
		getWorld()->increaseScore(50);
		getWorld()->getPeach()->giveShootPower();
		if (getWorld()->getPeach()->remainingHitPoints() < 2)
			getWorld()->getPeach()->incHitPoints();
		spend();
	}
}

////////////////////////////////////////
// MUSHROOM FUNCTIONS
////////////////////////////////////////

void Mushroom::doSomething()
{
	if (isDead())
		return;
	if (isReady())
	{
		this->moevement();
	}
	if (getWorld()->isTouchingPeach(getX(), getY()) && isReady())
	{
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		setDead();
		getWorld()->increaseScore(75);
		getWorld()->getPeach()->giveJumpPower();
		if(getWorld()->getPeach()->remainingHitPoints() < 2)
			getWorld()->getPeach()->incHitPoints();
		spend();
	}
}

////////////////////////////////////////
// STAR FUNCTIONS
////////////////////////////////////////

void Star::doSomething()
{
	if (isDead())
		return;
	if (isReady())
	{
		this->moevement();
	}
	if (getWorld()->isTouchingPeach(getX(), getY()) && isReady())
	{
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		setDead();
		getWorld()->increaseScore(100);
		getWorld()->getPeach()->makeInvincible();
		if (getWorld()->getPeach()->remainingHitPoints() < 2)
			getWorld()->getPeach()->incHitPoints();
		spend();
	}
}

////////////////////////////////////////
// GOOMBA FUNCTIONS
////////////////////////////////////////

void Goomba::doSomething()
{
	if (isDead())
		return;
	if (getWorld()->isTouchingPeach(getX(), getY()))
	{
		getWorld()->getPeach()->bonk();
		return;
	}
	if (getDirection() == 0)
	{
		if (getWorld()->canMoveHere(getX() + 1, getY()) == nullptr && getWorld()->isEmptyUnderneathEnemy(this, +SPRITE_WIDTH) != nullptr)
		{
			moveTo(getX() + 1, getY());
		}
		else
		{
			setDirection(180);
		}
	}
	else if (getDirection() == 180)
	{
		if (getWorld()->canMoveHere(getX() - 1, getY()) == nullptr && getWorld()->isEmptyUnderneathEnemy(this, -SPRITE_WIDTH) != nullptr)
		{
			moveTo(getX() - 1, getY());
		}
		else
		{
			setDirection(0);
		}
	}
}

void Goomba::bonk()
{
	getWorld()->increaseScore(100);
	getWorld()->playSound(SOUND_PLAYER_KICK);
	setDead();
}

void Goomba::getDamaged()
{
	getWorld()->increaseScore(100);
	setDead();
}

////////////////////////////////////////
// KOOPA FUNCTIONS
////////////////////////////////////////

void Koopa::doSomething()
{
	if (isDead())
		return;
	if (getWorld()->isTouchingPeach(getX(), getY()))
	{
		getWorld()->getPeach()->bonk();
		return;
	}
	if (getDirection() == 0)
	{
		if (getWorld()->canMoveHere(getX() + 1, getY()) == nullptr && getWorld()->isEmptyUnderneathEnemy(this, +SPRITE_WIDTH) != nullptr)
		{
			moveTo(getX() + 1, getY());
		}
		else
		{
			setDirection(180);
		}
	}
	else if (getDirection() == 180)
	{
		if (getWorld()->canMoveHere(getX() - 1, getY()) == nullptr && getWorld()->isEmptyUnderneathEnemy(this, -SPRITE_WIDTH) != nullptr)
		{
			moveTo(getX() - 1, getY());
		}
		else
		{
			setDirection(0);
		}
	}
}

void Koopa::bonk()
{
	Shell* s = new Shell(getX(), getY(), getWorld(), getDirection());
	s->makePassable();
	getWorld()->add_to_actors(s);
	getWorld()->increaseScore(100);
	getWorld()->playSound(SOUND_PLAYER_KICK);
	setDead();
}

void Koopa::getDamaged()
{
	Shell* s = new Shell(getX()/SPRITE_WIDTH, getY()/SPRITE_HEIGHT, getWorld(), getDirection());
	s->makePassable();
	getWorld()->add_to_actors(s);
	getWorld()->increaseScore(100);
	setDead();
}

////////////////////////////////////////
// PIRANHA FUNCTIONS
////////////////////////////////////////

void Piranha::doSomething()
{
	if (isDead())
		return;
	if (
		getY() < getWorld()->getPeach()->getY() - (1.5 * SPRITE_HEIGHT)
		|| getY() > getWorld()->getPeach()->getY() + (1.5 * SPRITE_HEIGHT)
		)
		return;
	if (m_firingDelay != 0)
	{
		m_firingDelay--;
		return;
	}
	if (getWorld()->isTouchingPeach(getX(), getY()))
	{
		getWorld()->getPeach()->bonk();
		return;
	}
	if (getWorld()->getPeach()->getX() < getX()) setDirection(180);
	else if (getWorld()->getPeach()->getX() > getX()) setDirection(0);
	increaseAnimationNumber();
	if (
		getWorld()->getPeach()->getX() > getX() - (8 * SPRITE_WIDTH)
		&& getWorld()->getPeach()->getX() < getX() + (8 * SPRITE_WIDTH)
		)
	{
		PiranhaFireball* pf = new PiranhaFireball(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT, getWorld(), getDirection());
		pf->makePassable();
		getWorld()->add_to_actors(pf);
		getWorld()->playSound(SOUND_PIRANHA_FIRE);
		m_firingDelay = 40;
	}
}

void Piranha::bonk()
{
	getWorld()->increaseScore(100);
	getWorld()->playSound(SOUND_PLAYER_KICK);
	setDead();
}

void Piranha::getDamaged()
{
	getWorld()->increaseScore(100);
	setDead();
}

////////////////////////////////////////
// PIRANHA FIREBALL FUNCTIONS
////////////////////////////////////////

void PiranhaFireball::doSomething()
{
	if (getWorld()->isTouchingPeach(getX(), getY()))
	{
		getWorld()->getPeach()->bonk();
		setDead();
		return;
	}
	if (getWorld()->isEmptyUnderneath(this) == nullptr)
	{
		moveTo(getX(), getY() - 2);
	}
	if (getDirection() == 0)
	{
		if (getWorld()->canMoveHere(getX() + 2, getY()) == nullptr)
		{
			moveTo(getX() + 2, getY());
		}
		else
		{
			setDead();
			return;
		}
	}
	else if (getDirection() == 180)
	{
		if (getWorld()->canMoveHere(getX() - 2, getY()) == nullptr)
		{
			moveTo(getX() - 2, getY());
		}
		else
		{
			setDead();
			return;
		}
	}
}

////////////////////////////////////////
// PEACH FIREBALL FUNCTIONS
////////////////////////////////////////

void PeachFireball::doSomething()
{
	Actor* a = getWorld()->isTouchingActor(getX(), getY());
	if (a != nullptr)
	{
		a->getDamaged();
		setDead();
		return;
	}
	if (getWorld()->isEmptyUnderneath(this) == nullptr)
	{
		moveTo(getX(), getY() - 2);
	}
	if (getDirection() == 0)
	{
		if (getWorld()->canMoveHere(getX() + 2, getY()) == nullptr)
		{
			moveTo(getX() + 2, getY());
		}
		else
		{
			setDead();
			return;
		}
	}
	else if (getDirection() == 180)
	{
		if (getWorld()->canMoveHere(getX() - 2, getY()) == nullptr)
		{
			moveTo(getX() - 2, getY());
		}
		else
		{
			setDead();
			return;
		}
	}
}

////////////////////////////////////////
// SHELL FUNCTIONS
////////////////////////////////////////

void Shell::doSomething()
{
	Actor* a = getWorld()->isTouchingActor(getX(), getY());
	if (a != nullptr)
	{
		a->getDamaged();
		setDead();
		return;
	}
	if (getWorld()->isEmptyUnderneath(this) == nullptr)
	{
		moveTo(getX(), getY() - 2);
	}
	if (getDirection() == 0)
	{
		if (getWorld()->canMoveHere(getX() + 2, getY()) == nullptr)
		{
			moveTo(getX() + 2, getY());
		}
		else
		{
			setDead();
			return;
		}
	}
	else if (getDirection() == 180)
	{
		if (getWorld()->canMoveHere(getX() - 2, getY()) == nullptr)
		{
			moveTo(getX() - 2, getY());
		}
		else
		{
			setDead();
			return;
		}
	}
}