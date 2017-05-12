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
	int numOfSonarTicks();
	bool isThereDirtVisibleHere(int x, int y);
	bool DMinVicinity(int range, int x, int y);
	bool ProtesterinVicinity(int range, int x, int y, char type);
	int dmXlocation() { return dm->getX(); }
	int dmYlocation() { return dm->getY(); }
	void generateField(string type);
	int randYGenerator(string type);
	int randXGenerator();
	bool goodSpot(int randX, int randY);
	bool farAway(int x, int y);
	void dropNugget();
	void decrementGoldBait();
	void incrementGoldBait();
	int numOfGoldBait();
	void decrementSonarKit();
	void incrementSonarKit();
	int numOfSonarKits();
	void sonarBLAST();
	bool isDirtAboveMe(int x, int y, int z);
	bool isDirtLeftOfMe(int x, int y, int z);
	bool isDirtRightOfMe(int x, int y, int z);
	bool isDirtUnderMe(int x, int y, int z);
	bool getDistDigManOnX(int x, int y, int& dis);
	bool getDistDigManOnY(int x, int y, int& dis);
	bool canShout(int x, int y);
	bool isMoveableLocForProtester(int x, int y);
	bool isABoulderHere(int x, int y);
	void addSquirtWeapon(Squirt* s);
private:
	int SonarKits;
	int currentKey;
	int GoldBait; //note : static variables initalized to zero by default
	vector<Actor *> actors;
	DiggerMan* dm;
	Dirt* dirt[64][64];
	Protester* p;
};

#endif // STUDENTWORLD_H_
