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
		else if (getY() > 0 && !getWorld()->isABoulderHere(getX(), getY() -1 ))
			moveTo(getX(), getY() - 1);
		break;
	case KEY_PRESS_LEFT:
		if (getDirection() != left)
			setDirection(left);
		else if (getX() > 0 && !getWorld()->isABoulderHere(getX() - 1, getY()))
			moveTo(getX() - 1, getY());
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right)
			setDirection(right);
		else if (getX() < VIEW_WIDTH - 4 && !getWorld()->isABoulderHere(getX() + 1, getY()))
			moveTo(getX() + 1, getY());
		break;
	case KEY_PRESS_UP:
		if (getDirection() != up)
			setDirection(up);
		else if (getY() < VIEW_HEIGHT - 4 && !getWorld()->isABoulderHere(getX(), getY() + 1))
			moveTo(getX(), getY() + 1);
		break;
	case KEY_PRESS_SPACE:
		if (getWorld()->numOfGoldBait() > 0) {
			getWorld()->dropNugget();
			getWorld()->decrementGoldBait();
		}
		break;
	case KEY_PRESS_TAB:
		
		if (getWorld()->numOfSonarKits() > 0) {
			getWorld()->playSound(SOUND_SONAR);
			getWorld()->sonarBLAST();
		}
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
		if (getHealth() < 0 && waitCount <=0)
			currentState = annoyed;
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
			if(!checkIfCanSeeDigMan())
				moveProtester();
			if (yellCoolDown == 0) {
				if(getWorld()->canShout(getX(), getY()));
					yellCoolDown = 15;
			}
			break;
		case annoyed:   //annoyed state, goBackToSafeSpace()
			goBackToSafeSpace();
			break;
		case follow:    //following diggerman
			break;
		case rest:		//resting state, either between ticks, or when annoyed
			if (waitCount < 1)
				currentState = move;
			waitCount--;
			break;
		}
		if( yellCoolDown != 0)
			yellCoolDown--;
	}
}
bool Protester::checkIfCanSeeDigMan() {
	int x;
	if (getWorld()->getDistDigManOnX(getX(), getY(), x)) {
		if (x < 0) {
			for (int i = 0; i > x; i--) {
				if (getWorld()->isDirtLeftOfMe(getX(), getY(), i)) 
					return false;
			}
			followDMHelper(getX() - 1, getY(), left);
			return true;
		}
		if (x > 0) {
			for (int i = 0; i < x; i++) {
				if (getWorld()->isDirtRightOfMe(getX(), getY(), i)) 
					return false;
			}
			followDMHelper(getX() + 1, getY(), right);
			return true;
		}
	}
	int y;
	if (getWorld()->getDistDigManOnY(getX(), getY(), y)) {
		if (y < 0) {
			for (int i = 0; i > y; i--) {
				if (getWorld()->isDirtUnderMe(getX(), getY(), i))
					return false;
			}
			followDMHelper(getX(), getY() - 1, down);
			return true;
		}
		if (y > 0) {
			for (int i = 0; i < y; i++) {
				if (getWorld()->isDirtAboveMe(getX(), getY(), i))
					return false;
			}
			followDMHelper(getX(), getY() + 1, up);
			return true;
		}
	}

}
void Protester::followDMHelper(int x, int y, Direction d) {
	setDirection(d);
	moveTo(x, y);
	waitCount = getTicksBetweenMoveCount();
	moveCount = getRandomDirMoveTickCount();
	currentState = rest;
}
void Protester::goBackToSafeSpace(){
	if (quickPathFound) {
		int u = 100000,d = 100000, l = 100000, r = 100000;
		if (getY() + 1 < 64 && bfsArray[getX()][getY() + 1] != -2)
			u = bfsArray[getX()][getY() + 1];
		if (getY() - 1 >= 0 && bfsArray[getX()][getY() - 1] != -2)
			d = bfsArray[getX()][getY() - 1];
		if (getX() + 1 < 64 && bfsArray[getX() + 1][getY()] != -2)
			r = bfsArray[getX() + 1][getY()];
		if (getX() - 1 >= 0 && bfsArray[getX() - 1][getY()] != -2) 
			l = bfsArray[getX() - 1][getY()];
		if (u <= r && u <= l && u <= d)
			moveTo(getX(), getY() + 1);
		else if (r <= u && r <= l && r <= d)
			moveTo(getX() + 1, getY());
		else if (l <= u && l <= r && l <= d)
			moveTo(getX() - 1, getY());
		else if(d <= u && d <= r && d <= l)
			moveTo(getX(), getY() - 1);
		waitCount = getTicksBetweenMoveCount();
		currentState = rest;
	}
	else {
		for (int i = 0; i < VIEW_WIDTH; i++) {
			for (int j = 0; j < VIEW_HEIGHT; j++) {
				if (!getWorld()->isMoveableLocForProtester(i, j) || getWorld()->isABoulderHere(i, j))
					bfsArray[i][j] = -2;
				else
					bfsArray[i][j] = -1;
			}
		}	
		bfsArray[60][60] = 0;
		bfsQueue.push(make_pair(make_pair(60, 60), 0));
		while (!bfsQueue.empty()) {
			pair<pair<int, int>, int> p = bfsQueue.front();
			bfsQueue.pop();
			//if surrounding nodes are in bounds, and not visited, then push to queue
			if (p.first.first + 1 < VIEW_WIDTH && bfsArray[p.first.first + 1][p.first.second] == -1)
				bfsQueue.push(make_pair(make_pair(p.first.first + 1, p.first.second), p.second + 1));
			if (p.first.first - 1 >= 0 && bfsArray[p.first.first - 1][p.first.second] == -1)
				bfsQueue.push(make_pair(make_pair(p.first.first - 1, p.first.second), p.second + 1));
			if (p.first.second - 1 >= 0 && bfsArray[p.first.first][p.first.second - 1] == -1)
				bfsQueue.push(make_pair(make_pair(p.first.first, p.first.second - 1), p.second + 1));
			if (p.first.second + 1 < VIEW_HEIGHT && bfsArray[p.first.first][p.first.second + 1] == -1)
				bfsQueue.push(make_pair(make_pair(p.first.first, p.first.second + 1), p.second + 1));
			bfsArray[p.first.first][p.first.second] = p.second;
		}
		quickPathFound = true;

	}
}
int Protester::getTicksBetweenMoveCount(){return max(0, 3 - (int)getWorld()->getLevel() / 4);}
int Protester::getRandomDirMoveTickCount(){return (rand() % 52) + 8;}

void Protester::protesterMoveHelper(int x, int y){
	moveTo(getX() + x, getY() + y);
	moveCount--;
}
void Protester::moveProtester(){
	if (waitCount < 1 && moveCount > 0) {
		if (getDirection() == left && getX() > 0 && !getWorld()->isDirtLeftOfMe(getX(), getY(), 0))
			protesterMoveHelper(-1, 0);
		else if (getDirection() == right && getX() < VIEW_WIDTH && !getWorld()->isDirtRightOfMe(getX(), getY(), 0))
			protesterMoveHelper(1, 0);
		else if (getDirection() == down && getY() > 0 && !getWorld()->isDirtUnderMe(getX(), getY(), 0))
			protesterMoveHelper(0, -1);
		else if (getDirection() == up && getY() < VIEW_HEIGHT - 4 && !getWorld()->isDirtAboveMe(getX(), getY(), 0))
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
	if (getTicksLeftTillDeath() == 0) {
		this->setVisible(false);
		this->kill();
	}
	else decreaseLifeTicks();
}
//=====Perm Nugget=====

void PermGoldNugget::GoldPickedUp() { found = true; }							//gold was found
void PermGoldNugget::doSomething() {
	if (!isAlive())
		return;
	if (getWorld()->DMinVicinity(4, getX(), getY())) {
		this->setVisible(true);
	}
	if (getWorld()->DMinVicinity(3, getX(), getY())) {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->incrementGoldBait();
		GoldPickedUp();
		this->setVisible(false);
		found = true;
		this->kill();
	}
}


/*
----------------------------
BOULDER IMPLEMENTATION
----------------------------
*/
void Boulder::doSomething() {
	switch (currentState) {
	case falling:
		if (getY() != 0 && !getWorld()->isDirtUnderMe(getX(), getY(), 0))
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
		if (!getWorld()->isDirtUnderMe(getX(), getY(), 0))
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
	if (getWorld()->DMinVicinity(4, getX(), getY())) {
		this->setVisible(true);
	}
	if (getWorld()->DMinVicinity(3, getX(), getY())) {
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		this->setVisible(false);
		this->kill();
	}
}

/*
----------------------------
SONAR KIT IMPLEMENTATION
----------------------------
*/

void Sonar::doSomething() {
	if (!isAlive())
		return;
	if (getWorld()->DMinVicinity(3, getX(), getY())) {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(75);
		getWorld()->incrementSonarKit();
		this->setVisible(false);
		this->kill();
	}
	else if (current_ticks() == 0) {
		this->setVisible(false);
		this->kill();
	}
	decrement_tick();
}

int Sonar::current_ticks() { return ticks; }
void Sonar::decrement_tick() { if (ticks > 0) ticks--; }
void Sonar::set_ticks() { ticks = getWorld()->numOfSonarTicks(); }
