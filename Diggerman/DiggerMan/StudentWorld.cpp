#include "StudentWorld.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


int StudentWorld::init() {
	currentKey = -1;
	dm = new DiggerMan(this);
	dm->setVisible(true);
	b = new Boulder(45, 30, this);
	b->setVisible(true);
	fillDirt();
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move() {
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	HUD();
	getKey(currentKey);
	if (currentKey == KEY_PRESS_ESCAPE) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	b->doSomething();
	dynamic_cast<DiggerMan*>(dm)->doSomething();
	currentKey = 0;
	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::removeDirt(int x, int y) {
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (dirt[i][j]) {
				Dirt* temp = dirt[i][j];
				dirt[i][j] = nullptr;
				delete temp;
			}
		}
	}
}


void StudentWorld::HUD() {
	string HUD =
		"Lvl: " + to_string(getLevel()) + " " +
		"Lives: " + to_string(getLives()) + " " +
		"Hlth: " + to_string(dm->getHealth() * 100) + "% " +
		"Scr: " + to_string(getScore()) + " ";
	setGameStatText(HUD);
}
int StudentWorld::getCurKey() { return currentKey; }

//the following determine how many of each Goodie will be in the current level:
int StudentWorld::numOfGoldNuggets() { return min((int)(5 - getLevel()) / 2, 2); }
int StudentWorld::numOfBoulders() { return min((int)(getLevel()) / 4, 7); }
int StudentWorld::numOfOilBarrels() { return min((int)(2 + getLevel()), 18); }

bool StudentWorld::isThereDirtVisibleHere(int x, int y){
	if (dirt[x][y])
		return true;
	return false;
}

void StudentWorld::fillDirt(){
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < VIEW_HEIGHT; j++) {
			dirt[i][j] = new Dirt(i, j, this);
			if ((i >= 30 && i <= 33 && j >= 4) || (j >= VIEW_HEIGHT - 4))
				dirt[i][j] = nullptr;
			else
				dirt[i][j]->setVisible(true);
		}
	}
}

void StudentWorld::cleanUp() {}

