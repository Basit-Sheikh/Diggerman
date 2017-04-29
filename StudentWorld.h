#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include "GraphObject.h"
#include <algorithm>
#include<ctime>
#include <vector>
using namespace std;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{

	}
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	int getCurKey();
	void removeDirt(int x, int y);
	int numOfBoulders();
	int numOfGoldNuggets();
	int numOfOilBarrels();
private:
	int currentKey;
	DiggerMan* dm;
	Dirt* dirt[64][64];

};

#endif // STUDENTWORLD_H_
