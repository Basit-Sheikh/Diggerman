#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;


class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw) :
		GraphObject(imageID, startX, startY, dir, size, depth), world(sw) {};
	virtual void doSomething() {};
	StudentWorld* getWorld() const { return world; }
private:
	StudentWorld* world;
	bool isDiggerManNearMe(int x,int y);
};


class Dirt : public Actor {
public:
	Dirt(int startX, int startY, StudentWorld* sw) : Actor(IMID_DIRT, startX, startY, right, .25, 3, sw) {};
	virtual void doSomething() {};
};


class DiggerMan : public Actor {
public:
	DiggerMan(StudentWorld* sw) : Actor(IMID_PLAYER, 30, 60, right, 1.0, 0, sw) { health = 1; };
	virtual void doSomething();
	virtual int getHealth();
private:
	void moveDiggerMan();
	void clearDirt();
	int health;
};


class Goodies :public Actor {
public:
	Goodies(StudentWorld* sw, const int img, int randX, int randY) :Actor(img, randX, randY, right, 1.0, 2, sw) {};
	bool isAlive() { return alive; }
	void setDead() { alive = false; }; //set bool to false
	virtual void doSomething() {};
private:
	bool alive = true;
};


class Boulder : public Actor {
public:
	Boulder(int x, int y, StudentWorld* sw) : Actor(IMID_BOULDER, x, y, down, 1.0, 1, sw), currentState(stable), tickCount(0){ };
	virtual void doSomething();
private:
	enum State { stable, falling, waiting, done };
	State currentState;
	int tickCount;
	bool isDirtUnderMe();

};

class Barrel : public Goodies{
public:
	Barrel(int randX, int randY, StudentWorld* sw) : Goodies(sw, IMID_BARREL, randX, randY){};
	virtual void doSomething();
private:
	
};


class Protester : public Actor {};


class HardcoreProtester : public Protester {};


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
