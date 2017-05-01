#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <algorithm>
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
private:
	int currentKey;
	DiggerMan* dm;
	Dirt* dirt[64][64];
	Boulder* b;
	Barrel* oil_barrel;
};

#endif // STUDENTWORLD_H_
