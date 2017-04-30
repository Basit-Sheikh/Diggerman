#include "Actor.h"
#include "StudentWorld.h"

/*
----------------------------
ACTOR IMPLEMENTATION
----------------------------
*/
bool Actor::isDiggerManNearMe() {
	return true;
}
/*
----------------------------
DIGGERMAN IMPLEMENTATION
----------------------------
*/

void DiggerMan::doSomething() {
	if (getWorld()->getLives() > 0) {
		moveDiggerMan();
		clearDirt();
	}
}

void DiggerMan::moveDiggerMan() {
	int dir = getWorld()->getCurKey();
	switch (dir) {
	case KEY_PRESS_DOWN:
		if (getDirection() != down)
			setDirection(down);
		else if (getY() > 0)
			moveTo(getX(), getY() - 1);
		break;
	case KEY_PRESS_LEFT:
		if (getDirection() != left)
			setDirection(left);
		else if (getX() > 0)
			moveTo(getX() - 1, getY());
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right)
			setDirection(right);
		else if (getX() < VIEW_WIDTH - 4)
			moveTo(getX() + 1, getY());
		break;
	case KEY_PRESS_UP:
		if (getDirection() != up)
			setDirection(up);
		else if (getY() < VIEW_HEIGHT - 4) 
			moveTo(getX(), getY() + 1);
		break;
	}
}
void DiggerMan::clearDirt() { getWorld()->removeDirt(getX(), getY()); }
int DiggerMan::getHealth() { return health; }

/*
----------------------------
NUGGET IMPLEMENTATION
----------------------------
*/

//=====Temp Nugget=====
int TempGoldNugget::getTicksLeftTillDeath() { return ticksLeftTillDeath; }		//returns how many ticks i have till this object dies
void TempGoldNugget::decreaseLifeTicks() { ticksLeftTillDeath--; }				//decreases their ticks by one
void TempGoldNugget::doSomething() {
	if (getTicksLeftTillDeath() == 0)
		setDead();
}
//=====Perm Nugget=====
bool PermGoldNugget::DmInVicinity(int x, int y) { return true; }				//pass in the getX and getY from the pointer to the actor
void PermGoldNugget::GoldPickedUp() { found = true; }							//gold was found


/*
----------------------------
BOULDER IMPLEMENTATION
----------------------------
*/
void Boulder::doSomething() {
	switch (currentState) {
	case falling:
		if (getY() != 0 && !isDirtUnderMe())
			moveTo(getX(), getY() - 1);
		else {
			currentState = done;
			setVisible(false);
			getWorld()->playSound(SOUND_NONE);
		}
		break;
	case waiting:
		if (tickCount == 30) {
			getWorld()->playSound(SOUND_FALLING_ROCK);
			currentState = falling;
			getWorld()->removeDirt(getX(), getY());
		}
		else
			tickCount++;
		break;
	case stable:
		if (!isDirtUnderMe())
			currentState = waiting;
		break;
	case done:
		break;
	}
}


bool Boulder::isDirtUnderMe()
{
	if (getWorld()->isThereDirtVisibleHere(getX(), getY() - 1) || getWorld()->isThereDirtVisibleHere(getX() + 1 , getY() - 1) ||
		getWorld()->isThereDirtVisibleHere(getX() + 2, getY() - 1) || getWorld()->isThereDirtVisibleHere(getX() + 3, getY() - 1)) {
		return true;
	}
	return false;
}
