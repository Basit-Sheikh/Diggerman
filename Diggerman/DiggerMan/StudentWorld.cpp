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
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < VIEW_HEIGHT - 4; j++) {
			dirt[i][j] = new Dirt(i, j, this);
			if (i >= 30 && i <= 33 && j >= 4 && j < VIEW_HEIGHT - 4)
				dirt[i][j]->setVisible(false);
			else
				dirt[i][j]->setVisible(true);
		}
	}
	b = new Boulder(45, 30, this);
	b->setVisible(true);
	for (int i = 0; i < 4; i++)					//removes dirt from whatever position the boulder is at
		for (int j = 0; j < 4; j++)
			dirt[45+i][30+j]->setVisible(false);
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
	if (x < 61 && y < 57)
		for (int i = x; i < x + 4; i++)
			for (int j = y; j < y + 4; j++)
				dirt[i][j]->setVisible(false);
	else if (x < 61 && y == 57)
		for (int i = x; i < x + 4; i++)
			for (int j = y; j < y + 3; j++)
				dirt[i][j]->setVisible(false);
	else if (x < 61 && y == 58)
		for (int i = x; i < x + 4; i++)
			for (int j = y; j < y + 2; j++)
				dirt[i][j]->setVisible(false);
	else if (x < 61 && y == 59)
		for (int i = x; i < x + 4; i++)
			for (int j = y; j < y + 1; j++)
				dirt[i][j]->setVisible(false);
	else;
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

bool StudentWorld::isThereDirtVisibleHere(int x, int y){ return dirt[x][y]->isVisible(); }

void StudentWorld::cleanUp() {}

