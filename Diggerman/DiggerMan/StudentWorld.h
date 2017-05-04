#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
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
	void fillDirt();
	void removeDirt(int x, int y);
	void HUD();
	int numOfBoulders();
	int numOfGoldNuggets();
	int numOfOilBarrels();
	bool isThereDirtVisibleHere(int x, int y);
	bool DMinVicinity(int range, Actor *a);
	int dmXlocation() { return dm->getX(); }
	int dmYlocation() { return dm->getY(); }
	void generateField(string type);

private:
	int currentKey;
	vector<Actor *> actors;
	DiggerMan* dm;
	Dirt* dirt[64][64];
	Protester* p;
};

#endif // STUDENTWORLD_H_
