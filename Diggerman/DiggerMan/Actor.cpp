#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

/*
----------------------------
ACTOR IMPLEMENTATION
----------------------------
*/
bool Actor::isDiggerManNearMe(int x, int y) { 
	if (sqrt(((x - getX()) ^ 2) + ((y - getY()) ^ 2)) <= 4)
		return true;
	
	return true; 
}
bool Actor::isDirtUnderMe(){
	return (getWorld()->isThereDirtVisibleHere(getX(), getY() - 1) || getWorld()->isThereDirtVisibleHere(getX() + 1, getY() - 1) ||
		getWorld()->isThereDirtVisibleHere(getX() + 2, getY() - 1) || getWorld()->isThereDirtVisibleHere(getX() + 3, getY() - 1)); 
}

/*
----------------------------
DIGGERMAN IMPLEMENTATION 
----------------------------
*/
void DiggerMan::doSomething() {
	if (getWorld()->getLives() > 0) {
		moveDiggerMan();
		//cout << getX() << " " << getY() << endl;         <--- good test to see diggermans position when testing ranges
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
/*
----------------------------
PROTESTER IMPLEMENTATION 
----------------------------
*/
void Protester::doSomething() {
	if (isAlive()) {
		//rest, move, annoyed, follow, start
		switch (currentState) {
		case start:		//init state
			moveCount = getRandomDirMoveTickCount();
			waitCount = getTicksBetweenMoveCount();
			moveTo(getX() - 1, getY());
			currentState = rest;
			break;
		case move:		//randomly moving state
			//moveProtester is a helper function for do something
			//just moves protester, moved to func for readablility of doSomething()
			moveProtester();
			break;
		case annoyed:   //annoyed state, goBackToSafeSpace()
			break;
		case follow:    //following diggerman
			break;
		case rest:		//resting state, either between ticks, or when annoyed
			if (waitCount < 1)
				currentState = move;
			waitCount--;

			break;

		}
	}
}
int Protester::getTicksBetweenMoveCount(){return max(0, 3 - (int)getWorld()->getLevel() / 4);}
int Protester::getRandomDirMoveTickCount(){return (rand() % 52) + 8;}
bool Protester::isDirtAboveMe() {
	return (getWorld()->isThereDirtVisibleHere(getX(), getY() + 4) || getWorld()->isThereDirtVisibleHere(getX() + 1, getY() + 4) ||
		getWorld()->isThereDirtVisibleHere(getX() + 2, getY() + 4) || getWorld()->isThereDirtVisibleHere(getX() + 3, getY() + 4));
}
bool Protester::isDirtLeftOfMe() {
	return (getWorld()->isThereDirtVisibleHere(getX() - 1, getY()) || getWorld()->isThereDirtVisibleHere(getX() - 1, getY() + 1) ||
		getWorld()->isThereDirtVisibleHere(getX() - 1, getY() + 2) || getWorld()->isThereDirtVisibleHere(getX() - 1, getY() + 3));
}
bool Protester::isDirtRightOfMe() {
	return (getWorld()->isThereDirtVisibleHere(getX() + 4, getY()) || getWorld()->isThereDirtVisibleHere(getX() + 4, getY() + 1) ||
		getWorld()->isThereDirtVisibleHere(getX() + 4, getY() + 2) || getWorld()->isThereDirtVisibleHere(getX() + 4, getY() + 3));
}
void Protester::protesterMoveHelper(int x, int y){
	moveTo(getX() + x, getY() + y);
	moveCount--;
}
void Protester::moveProtester(){
	if (waitCount < 1 && moveCount > 0) {
		if (getDirection() == left && getX() > 0 && !isDirtLeftOfMe())
			protesterMoveHelper(-1, 0);
		else if (getDirection() == right && getX() < VIEW_WIDTH && !isDirtRightOfMe())
			protesterMoveHelper(1, 0);
		else if (getDirection() == down && getY() > 0 && !isDirtUnderMe())
			protesterMoveHelper(0, -1);
		else if (getDirection() == up && getY() < VIEW_HEIGHT - 4 && !isDirtAboveMe())
			protesterMoveHelper(0, 1);
		else {
			setDirection(pickRandomDirection());
			moveCount = getRandomDirMoveTickCount();
		}
		currentState = rest;
		waitCount = getTicksBetweenMoveCount();
	}
	else {
		setDirection(pickRandomDirection());
		moveCount = getRandomDirMoveTickCount();
	}
}
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
		kill();
}
//=====Perm Nugget=====

void PermGoldNugget::GoldPickedUp() { found = true; }							//gold was found
void PermGoldNugget::doSomething() {
	if (!isAlive())
		return;
	if (getWorld()->DMinVicinity(4, this)) {
		this->setVisible(true);
	}
	if (getWorld()->DMinVicinity(3, this)) {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		this->setVisible(false);
		found = true;
		this->kill();
	}

			//else if (true) {
			//	//if is radius 3 away from diggerman		 --done
			//	//set to dead or found idk which			 --done
			//	//play sound got goodie						 --done
			//	//increase player score by 10                --done
			//	//tell diggerman he gota new nugget somehow
			//}
	}


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
			this->setVisible(false);
			this->kill();
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
	}
}

/*
----------------------------
BARREL IMPLEMENTATION
----------------------------
*/

void Barrel::doSomething(){
	if (!isAlive())
		return;
	if (getWorld()->DMinVicinity(4, this)) {
		this->setVisible(true);
	}
	if (getWorld()->DMinVicinity(3, this)) {
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		this->setVisible(false);
		this->kill();
	}
}


