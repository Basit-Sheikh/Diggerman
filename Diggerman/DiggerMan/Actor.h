#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;


class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw) :
		GraphObject(imageID, startX, startY, dir, size, depth), world(sw), alive(true) {};
	virtual void doSomething() {};
	StudentWorld* getWorld() const { return world; }
	bool isAlive() const { return alive; }
	virtual bool isRegProtester() { return false; }
	void kill() { alive = false; }	
protected:
private:
	StudentWorld* world;
    bool isDiggerManNearMe(int x,int y);
	bool alive;

};

class Character : public Actor {
public:
	Character(int id, int x, int y, StudentWorld* sw, Direction dir, double size, int dep, int hp) : 
		Actor(id, x, y, dir, size, dep, sw), health(hp) {}
	int getHealth() const { return health; }
	void decHealth(int subAm) { health -= subAm; }
private:
	int health;

};

class DiggerMan : public Character {
public:
	DiggerMan(StudentWorld* sw) : 
		Character(IMID_PLAYER, 30, 60, sw, right, 1.0, 0, 10) {};
	virtual void doSomething();
private:
	void moveDiggerMan();
	void clearDirt();
};

class Protester : public Character {
protected:
	enum State { rest, move, annoyed, follow, start };

public:
	Protester(StudentWorld * sw) :
		Character(IMID_PROTESTER, 60, 60, sw, left, 1.0, 0, 5), currentState(start), moveCount(0), waitCount(0), yellCoolDown(0), quickPathFound(false) {}
	virtual void doSomething();
	int getTicksBetweenMoveCount();
	int getRandomDirMoveTickCount();
	void goBackToSafeSpace();
	virtual bool isRegProtester() { return true;}
	void setStateAnnoyed() { currentState = annoyed; }
private:
	State currentState;
	int moveCount;
	int waitCount;
	int yellCoolDown;
	bool quickPathFound;
	Direction pickRandomDirection() {
		int i = rand() % 4;
		if (i == 0)
			return left;
		if (i == 1)
			return right;
		if (i == 2)
			return down;
		if (i == 3)
			return up;
		return none;
	}
    void protesterMoveHelper(int x, int y);
	void moveProtester();
	bool checkIfCanSeeDigMan();
	void followDMHelper(int x, int y, Direction d);
	queue<pair<pair<int, int>, int>> bfsQueue;
	int bfsArray[64][64];
	void print();
};
class Dirt : public Actor {
public:
	Dirt(int startX, int startY, StudentWorld* sw) :
		Actor(IMID_DIRT, startX, startY, right, .25, 3, sw) {};
	virtual void doSomething() {};
};

class Goodies :public Actor {
public:
	Goodies(StudentWorld* sw, const int img, int randX, int randY) :
		Actor(img, randX, randY, right, 1.0, 2, sw) {};

	virtual void doSomething() {};
private:

};


class Boulder : public Actor {
public:
	Boulder(int x, int y, StudentWorld* sw) : 
		Actor(IMID_BOULDER, x, y, down, 1.0, 1, sw), currentState(stable), tickCount(0){ };
	virtual void doSomething();
private:
	enum State { stable, falling, waiting, done };
	State currentState;
	int tickCount;

};

class Barrel : public Goodies{
public:
	Barrel(int randX, int randY, StudentWorld* sw) : 
		Goodies(sw, IMID_BARREL, randX, randY){};
	virtual void doSomething();
private:
	
};

class Sonar : public Goodies {
public:
	Sonar(int randX, int randY, StudentWorld* sw) :
		Goodies(sw, IMID_SONAR, randX, randY) {
		set_ticks();
	};
	virtual void doSomething();
	int  current_ticks();
	void set_ticks();
	void decrement_tick();
private:
	int ticks;
};



class TempGoldNugget : public Goodies {
public:
	TempGoldNugget(int deathTicks, int randX, int randY, StudentWorld* sw) :
		Goodies(sw, IMID_GOLD, randX, randY), ticksLeftTillDeath(deathTicks) { this->setVisible(true); };
	int getTicksLeftTillDeath();
	void decreaseLifeTicks();
	virtual void doSomething();

private:
	int ticksLeftTillDeath;

};


class PermGoldNugget : public Goodies {
public:
	PermGoldNugget(StudentWorld* sw, int randX, int randY) :
		Goodies(sw, IMID_GOLD, randX, randY) {};

	void GoldPickedUp();
	virtual void doSomething();
private:
	bool found = false;
};

class Squirt : public Actor { //cannot inherit from goodies because depth is 1 not 2
public:
	Squirt(StudentWorld* sw, Direction d, int x, int y) :
		Actor(IMID_WATER_SPURT, x, y, right, 1, 1, sw) {
		squirt_distance = 0;
	};
	virtual void doSomething();
	void incrementDistance() { squirt_distance++; }
private:
	int squirt_distance;
};


#endif // ACTOR_H_
