#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include "GraphObject.h"
#include <vector>
using namespace std;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
		dirt = vector<Dirt*>(3840);
	}

	virtual int init()
	{
		currentKey = -1;
		dm = new DiggerMan(this);
		dm->setVisible(true);
		for (int i = 0; i < VIEW_WIDTH; i++) {
			for (int j = 0; j < VIEW_HEIGHT-4; j++) {
				if (i >= 30 && i <= 33 && j >= 4 && j < VIEW_HEIGHT - 4){
					Dirt* temp = new Dirt(i, j, this);
					temp->setVisible(false);
					dirt.push_back(temp);

				}
				else{
					Dirt* temp = new Dirt(i, j, this);
					temp->setVisible(true);
					dirt.push_back(temp);
				}
			}
		}
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
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

	virtual void cleanUp(){
	}
	int getCurKey() {return currentKey;}


private:
	int currentKey;
	DiggerMan* dm;
	vector<Dirt*> dirt;
};

#endif // STUDENTWORLD_H_
