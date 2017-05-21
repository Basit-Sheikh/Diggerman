#include "StudentWorld.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir) { return new StudentWorld(assetDir); }
int StudentWorld::init() {
	currentKey = -1;
	GoldBait = 0; //Created to initialize the goldbait count to zero. This will also reset the amount of gold bait at the end of every level to zero -- not sure if that should be done. 
	              //if it is not meant to be that way, we can use a flag to make sure it only runs once.
	SonarKits = 1; //he is given 1 at the start of every level
	SquirtsRemaining = 5;
	dm = new DiggerMan(this);
	dm->setVisible(true);
	fillDirt();
	
	
	generateField("Barrel");
	generateField("PermNugget");
	generateField("Boulder");
	generateField("RegProtester");
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

	if (dm->getHealth() <= 0) {
		decLives();
		cleanUp();
		playSound(SOUND_PLAYER_GIVE_UP);
		return GWSTATUS_PLAYER_DIED;
	}
	dm->doSomething();
	for (Actor* a : actors){
		if (a->isAlive()) 
			a->doSomething();
	}
	for (int i = 0; i < actors.size(); i++) {
		if (!actors[i]->isAlive()) {
			delete actors[i];
			actors.erase(actors.begin() + i);
		}	
	}
	int G = getLevel() * 25 + 300;
	if (rand() % G + 1 == 1) {
		if (rand() % 5 + 1 == 1) {
			actors.push_back(new Sonar(0, 60, this));
			actors.back()->setVisible(true);
		}
		else {
			int randX = randXGenerator();
			int randY = randYGenerator("");		
			int ct = 0;
			while (true) {
				cout << "This is ct:" << ct << endl;
				if (dirtlessSpots(randX,randY) && !isABoulderHere(randX, randY)) { //sometimes on the 2nd or 3rd life, this will infinite loop
					break;
				}
				randX = randXGenerator();
				randY = randYGenerator("");
				ct++;
			}
			actors.push_back(new WaterPool(numOfSonarAndWaterTicks(), randX, randY, this)); 
			actors.back()->setVisible(true);
		}
	}
	//1 in G chance that either a sonar or water pool will spawn
	// 4/5 chance of it being a water pool, 1/5 being a sonar kit
	currentKey = 0;
	return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::dirtlessSpots(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			if ((isThereDirtVisibleHere(x + i, y + k)))
				return false;
		}
	}
	return true;
}
bool StudentWorld::DMinVicinity(int range, int x, int y) {
	double dist = sqrt(pow(dm->getX() - x, 2) + pow(dm->getY() - y, 2));
	return dist <= range;
}
bool StudentWorld::ProtesterinVicinity(int range, int x, int y, char type) {
	for (Actor* a : actors) {
		if (a->isRegProtester()) {
			double dist = sqrt(pow(a->getX() - x, 2) + pow(a->getY() - y, 2));
			if (dist <= range) {
				if (type == 'n') dynamic_cast<Protester*>(a)->decHealth(10);       //nugget bait
				else if (type == 's') dynamic_cast<Protester*>(a)->decHealth(2);   //squirt hit
				return true;
			}
			else return false;
		}
	}
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
bool StudentWorld::removeDirt(int x, int y) {
	bool dugDirt = false;
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (dirt[i][j]) {
				/*****************************************************
				Dig sound is kind of loud and somewhat masks other sound effects, comment out the playSound function call directly below to remove it.
				***************************************************/
				dugDirt = true;
				Dirt* temp = dirt[i][j];
				dirt[i][j] = nullptr;
				delete temp;
			}
		}
	}
	return dugDirt;
}
void StudentWorld::dropNugget() {
	actors.push_back(new TempGoldNugget(200, dm->getX(), dm->getY(), this));  //Adjust tempNugg life to whatever yall want
	actors.back()->setVisible(true);
	decrementGoldBait();
}
int StudentWorld::numOfGoldBait() {
	return GoldBait;
}
void StudentWorld::decrementGoldBait() {
	if (GoldBait > 0)
		GoldBait--;
}
void StudentWorld::incrementSonarKit() { SonarKits++; }
void StudentWorld::decrementSonarKit() {
	if (SonarKits > 0)
		SonarKits--;
}
int StudentWorld::numOfSonarKits() {
	return SonarKits;
}
void StudentWorld::sonarBLAST() { //activates all nuggets and barrels within a radius of 12
	//playSound(SOUND_SONAR); //not in spec but found in code
	playSound(SOUND_SONAR);
	for (Actor *a : actors) {
		if (DMinVicinity(12, a->getX(), a->getY()))
			a->setVisible(true);
	}
	decrementSonarKit();
}
void StudentWorld::incrementGoldBait() { GoldBait++; }
int StudentWorld::randXGenerator() {
	int x = rand() % 61;
	while (x > 25 && x < 35)
		x = rand() % 61;
	return x;
}
int StudentWorld::randYGenerator(string type) {
	int y = rand() % 57;
	if (type == "Boulder") { //boulders must spawn between y = 20 and y = 56
		while (y < 20 || y > 56) {
			y = rand() % 57;
		}
	}
	return y;
}
bool StudentWorld::goodSpot(int randX,int randY) {		
		if (isThereDirtVisibleHere(randX, randY)) {
			if (!isThereDirtVisibleHere(randX, randY + 3) || !isThereDirtVisibleHere(randX + 3, randY + 3) || !isThereDirtVisibleHere(randX + 3, randY)) {
				return false;
			}
			return true;
		}
		return false;
}


bool StudentWorld::farAway(int x,int y) {
	for (Actor* a : actors) {
		if( (sqrt(pow(x - a->getX(), 2) + pow(y - a->getY(), 2))) < 7) {  //6 unit space between each spawned object
			return false;
		}
	}
	return true;
}
void StudentWorld::generateField(string type){
	int spawn_amount;
	if      (type == "PermNugget") spawn_amount = numOfGoldNuggets();
	else if (type == "Barrel")     spawn_amount = numOfOilBarrels();
	else if (type == "Boulder")    spawn_amount = numOfBoulders();
	else if (type == "RegProtester") {
		actors.push_back(new Protester(this));
		actors.back()->setVisible(true);
		return;
	}
	//------------------------------------------------
	//Generating items in acceptable situations
	//-----------------------------------------------
	for (int i = 0; i < spawn_amount; i++) {
		int randX = randXGenerator();
		int randY = randYGenerator(type);
		int ct = 0;
		while (true) {
			if (ct > 200) {
				if (goodSpot(randX, randY))
					break;
			}
			if (goodSpot(randX, randY)) {
				if (farAway(randX, randY))
					break;
			}
			randX = randXGenerator();
			randY = randYGenerator(type);
			ct++;
		}
		
		if (type == "PermNugget") {
			actors.push_back(new PermGoldNugget(this, randX, randY)); 
			actors.back()->setVisible(true);
		} 
		else if (type == "Barrel") {
			actors.push_back(new Barrel(randX, randY, this)); 
			actors.back()->setVisible(true);
		}		
		else if (type == "Boulder") {
			actors.push_back(new Boulder(randX, randY, this));
			removeDirt(randX, randY);                          //remove dirt from where the boulder spawns
			actors.back()->setVisible(true);
		}
	}
}
void StudentWorld::HUD() {
	string HUD =
		"Lvl: " + to_string(getLevel()) + " " +
		"Lives: " + to_string(getLives()) + " " +
		"Hlth: " + to_string(dm->getHealth() * 10) + "% " +
		"Scr: " + to_string(getScore()) + " ";
	setGameStatText(HUD);
}
int StudentWorld::getCurKey() { return currentKey; }
//the following determine how many of each Goodie will be in the current level:
int StudentWorld::numOfGoldNuggets() { return max((int)(5 - getLevel()) / 2, 2); }
int StudentWorld::numOfBoulders() { return min((int)(getLevel()) / 2 + 2, 7); }
int StudentWorld::numOfOilBarrels() { return min((int)(2 + getLevel()), 18); }
int StudentWorld::numOfSonarAndWaterTicks() { return max(100, int(300-(10*getLevel()))); } //returns how many ticks until sonar kit disappears/expires
bool StudentWorld::isThereDirtVisibleHere(int x, int y){ return dirt[x][y]; }
void StudentWorld::cleanUp() {

	//delete dirt
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < VIEW_HEIGHT; j++) {
			Dirt* temp = dirt[i][j];
			dirt[i][j] = nullptr;
			delete temp;
		}
	}
	//delete actors
	for (int i = 0; i < actors.size(); i++) {
		delete actors[i];
		actors.erase(actors.begin() + i);
	}

	//delete diggerman last
	DiggerMan *temp2 = dm;
	dm = nullptr;
	delete temp2;
}
bool StudentWorld::isDirtAboveMe(int x, int y, int z) {
	return (isThereDirtVisibleHere(x, y + 4 + z) || isThereDirtVisibleHere(x + 1, y + 4 + z) ||
		isThereDirtVisibleHere(x + 2, y + 4 + z) || isThereDirtVisibleHere(x + 3, y + 4 + z));
}
bool StudentWorld::isDirtLeftOfMe(int x, int y, int z) {
	return (isThereDirtVisibleHere(x - 1 + z,  y) || isThereDirtVisibleHere(x - 1 + z, y + 1) ||
		isThereDirtVisibleHere(x - 1 + z, y + 2) || isThereDirtVisibleHere(x - 1 + z, y + 3));
}
bool StudentWorld::isDirtRightOfMe(int x, int y, int z) {
	return (isThereDirtVisibleHere(x + 4 + z, y) || isThereDirtVisibleHere(x + 4 + z, y + 1) ||
		isThereDirtVisibleHere(x + 4 + z, y + 2) || isThereDirtVisibleHere(x + 4 + z, y + 3));
}
bool StudentWorld::isDirtUnderMe(int x, int y, int z){
	return (isThereDirtVisibleHere(x, y - 1 + z) || isThereDirtVisibleHere(x + 1, y - 1 + z) ||
		isThereDirtVisibleHere(x + 2, y - 1 + z) || isThereDirtVisibleHere(x + 3, y - 1 + z)); 
}
bool StudentWorld::getDistDigManOnX(int x, int y, int & dis) {
	if (dm->getY() == y) {
		dis = dm->getX() - x;
		return true;
	}
	return false;
}
bool StudentWorld::getDistDigManOnY(int x, int y, int & dis) {
	if (dm->getX() == x) {
		dis = dm->getY() - y;
		return true;
	}
	return false;
}
bool StudentWorld::canShout(int x, int y){
	if (DMinVicinity(4, x, y)) {
		playSound(SOUND_PROTESTER_YELL);
		dm->decHealth(1);
		return true;
	}
	return false;
}
bool StudentWorld::isMoveableLocForProtester(int x, int y){
	if(y >= VIEW_HEIGHT || y < 0 || x < 0 || x >= VIEW_WIDTH)
		return false;
	else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (dirt[x + i][y + j]) {
					return false;
				}
			}
		}
	}
	return true;
}
bool StudentWorld::isABoulderHere(int x, int y, GraphObject::Direction d) {
	for (auto a : actors) {
		if (typeid(*a) == typeid(Boulder) && dynamic_cast<Boulder*>(a)->getState() != Boulder::State::falling) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (d != GraphObject::Direction::none) {
						if (d == GraphObject::Direction::right && a->getX() == x + 4 && a->getY() + i == y + j)
							return true;
						else if (d == GraphObject::Direction::left && a->getX() + 3 == x - 1 && a->getY() + i == y + j)
							return true;
						else if (d == GraphObject::Direction::down && a->getX() + i == x + j && a->getY() + 3 == y - 1)
							return true;
						else if (d == GraphObject::Direction::up && a->getX() + i == x + j && a->getY() == y + 4)
							return true;
					}
					else if (a->getX() + i == x && a->getY() + j == y)
						return true;
				}
			}
		}
	}
	return false;
}



void StudentWorld::addSquirtWeapon(GraphObject::Direction dir, int x, int y) { 
	Squirt *s = new Squirt(this, dir, x, y);
	actors.push_back(s); 
	actors.back()->setVisible(true);
	decrementSquirts();
}

void StudentWorld::incrementSquirts() { SquirtsRemaining += 5; }

int StudentWorld::getSquirtsRemaining() { return SquirtsRemaining; }
void StudentWorld::decrementSquirts() { SquirtsRemaining--; }


void StudentWorld::killProtestorsHere(int x, int y) {
		for (Actor *p : actors) {
			if (p->isRegProtester()) {
				if (isThereContact(p->getX(), p->getY(), x, y)) {
					dynamic_cast<Protester*>(p)->decHealth(100);
				}
			}
		}	
}

bool StudentWorld::isThereContact(int x, int y, int x2, int y2) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				for (int z = 0; z < 3; z++) {
					if (x + i == x2+k  && y + j == y2+z)
						return true;
				}
			}
		}
	}
	return false;
}





