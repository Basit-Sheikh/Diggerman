#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <algorithm>
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;


class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw) :
		GraphObject(imageID, startX, startY, dir, size, depth), world(sw), alive(true) {};
	virtual void doSomething() {};
	StudentWorld* getWorld() const { return world; }
	bool isAlive() const { return alive; }
	void kill() { alive = false; }	
protected:
	bool isDirtUnderMe();
private:
	StudentWorld* world;
    bool isDiggerManNearMe(int x,int y);
	bool alive;

};

class Character : public Actor {
public:
	Character(int id, int x, int y, StudentWorld* sw, Direction dir, double size, int dep, int hp) : Actor(id, x, y, dir, size, dep, sw), health(hp) {}
	int getHealth() const { return health; }
	void decHealth(int subAm) { health -= subAm; }
private:
	int health;

};

class DiggerMan : public Character {
public:
	DiggerMan(StudentWorld* sw) : Character(IMID_PLAYER, 30, 60, sw, right, 1.0, 0, 10) {};
	virtual void doSomething();
private:
	void moveDiggerMan();
	void clearDirt();
};

class Protester : public Character {
protected:
	enum State { rest, move, annoyed, follow, start};
	bool isDirtAboveMe();
	bool isDirtLeftOfMe();
	bool isDirtRightOfMe();
public:
	Protester(StudentWorld * sw) : Character(IMID_PROTESTER, 60, 60, sw, left, 1.0, 0, 5), currentState(start), restCount(0), moveCount(0), waitCount(0) {}
	virtual void doSomething();
	int getTicksBetweenMoveCount();	
private:
	State currentState;
	int restCount;
	int moveCount;
	int waitCount;
};








class Dirt : public Actor {
public:
	Dirt(int startX, int startY, StudentWorld* sw) : Actor(IMID_DIRT, startX, startY, right, .25, 3, sw) {};
	virtual void doSomething() {};
};

class Goodies :public Actor {
public:
	Goodies(StudentWorld* sw, const int img, int randX, int randY) :Actor(img, randX, randY, right, 1.0, 2, sw) {};

	virtual void doSomething() {};
private:

};


class Boulder : public Actor {
public:
	Boulder(int x, int y, StudentWorld* sw) : Actor(IMID_BOULDER, x, y, down, 1.0, 1, sw), currentState(stable), tickCount(0){ };
	virtual void doSomething();
private:
	enum State { stable, falling, waiting, done };
	State currentState;
	int tickCount;

};

class Barrel : public Goodies{
public:
	Barrel(int randX, int randY, StudentWorld* sw) : Goodies(sw, IMID_BARREL, randX, randY){};
	virtual void doSomething();
private:
	
};


class TempGoldNugget : public Goodies {
public:
	TempGoldNugget(int deathTicks, int randX, int randY, StudentWorld* sw) :Goodies(sw, IMID_GOLD, randX, randY), ticksLeftTillDeath(deathTicks) { this->setVisible(true); };
	int getTicksLeftTillDeath();
	void decreaseLifeTicks();
	virtual void doSomething();

private:
	int ticksLeftTillDeath;

};


class PermGoldNugget : public Goodies {
public:
	PermGoldNugget(StudentWorld* sw, int randX, int randY) :Goodies(sw, IMID_GOLD, randX, randY) {};

	bool DmInVicinity(int x, int y);
	void GoldPickedUp();
	virtual void doSomething(); //uses the DmInvicinty function to see if DM is in vicinty and then uses GoldPickedUp to indicate that the gold has been found
private:
	bool found = false;
};



#endif // ACTOR_H_
