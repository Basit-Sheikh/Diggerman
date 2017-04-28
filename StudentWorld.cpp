#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init(){
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
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	getKey(currentKey);
	if (currentKey == KEY_PRESS_ESCAPE) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	dynamic_cast<DiggerMan*>(dm)->doSomething();


	currentKey = 0;
	return GWSTATUS_CONTINUE_GAME;

}

void StudentWorld::removeDirt(int x, int y){
	cout << x << " " << y << endl;
	if (x < 61 && y < 57)
		for (int i = x; i < x + 4; i++)
			for (int j = y; j < y + 4; j++)
				dirt[i][j]->setVisible(false);
	else if (x < 61 && y == 57)
		for (int i = x; i < x + 4 ; i++)
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


int StudentWorld::getCurKey() { return currentKey; }
void StudentWorld::cleanUp(){}
