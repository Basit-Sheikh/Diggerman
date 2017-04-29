#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void DiggerMan::doSomething(){
	if (getWorld()->getLives() > 0) {
		moveDiggerMan();
		clearDirt(this);
	}
}

void DiggerMan::moveDiggerMan() {
	int dir = getWorld()->getCurKey();
	if (dir == KEY_PRESS_DOWN) {
		if (getDirection() != down) {
			setDirection(down);
		}
		else {
			if (getY() > 0) {
				moveTo(getX(), getY() - 1);
			}
		}
	}
	if (dir == KEY_PRESS_UP) {
		if (getDirection() != up) {
			setDirection(up);
		}
		else {
			if (getY() < VIEW_HEIGHT - 4) {
				moveTo(getX(), getY() + 1);
			}
		}
	}
	if (dir == KEY_PRESS_LEFT) {
		if (getDirection() != left) {
			setDirection(left);
		}
		else {
			if (getX() > 0) {
				moveTo(getX() - 1, getY());
			}
		}
	}
	if (dir == KEY_PRESS_RIGHT) {
		if (getDirection() != right) {
			setDirection(right);
		}
		else {
			if (getX() < VIEW_WIDTH - 4) {
				moveTo(getX() + 1, getY());
			}
		}
	}
}

void DiggerMan::clearDirt(DiggerMan *dg){
	int x = dg->getX();
	int y = dg->getY();
	getWorld()->removeDirt(x, y);

}


void TempGoldNugget::doSomething() {
	if (getTicksLeftTillDeath() == 0)
		setDead(); 
	

}
