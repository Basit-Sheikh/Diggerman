#ifndef ACTOR_H_
#define ACTOR_H_

//#include "StudentWorld.h"
#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw) : 
		GraphObject(imageID, startX, startY, dir, size, depth), world(sw){};
	virtual void doSomething() {};
	StudentWorld* getWorld() const { return world; }
private:
	StudentWorld* world;
};

class Dirt : public Actor {
public:
	Dirt(int startX, int startY, StudentWorld* sw) : Actor(IMID_DIRT, startX, startY, right, .25, 3, sw){

	};
	virtual void doSomething() {};
};
class DiggerMan : public Actor {
public:
	DiggerMan(StudentWorld* sw) : Actor(IMID_PLAYER, 30, 60, right, 1.0, 0, sw) {};
	virtual void doSomething();
private:
	void moveDiggerMan();
	void clearDirt(DiggerMan *dg);
};
class Boulder : public Actor {

};
class Protester : public Actor {

};
class HardcoreProtester : public Protester {

};

class Goodies :public Actor {
public:
	Goodies(StudentWorld* sw,const int img, int randX, int randY) :Actor(img, randX, randY, right, 1.0, 2, sw) {}
	void setDead() { alive = false; }; //set bool to false
	virtual void doSomething() {};

private:
	bool alive = true;
};

class TempGoldNugget :public Goodies {
public:
	TempGoldNugget(int deathTicks, int randX, int randY, StudentWorld* sw) :Goodies(sw, IMID_GOLD, randX, randY), ticksLeftTillDeath(deathTicks) { this->setVisible(true); }
	int getTicksLeftTillDeath() { return ticksLeftTillDeath; } //returns how many ticks i have till this object dies
	void decreaseLifeTicks() { ticksLeftTillDeath--; } //decreases their ticks by one
	virtual void doSomething();

private:
	int ticksLeftTillDeath;

};

class PermGoldNugget :public Goodies {
public:
PermGoldNugget(StudentWorld* sw,int randX, int randY):Goodies(sw,IMID_GOLD,randX,randY){}

bool DmInVicinity(int x, int y) { return true; }//pass in the getX and getY from the pointer to the actor
void GoldPickedUp() { found = true; } //gold was found
virtual void doSomething() {}; //uses the DmInvicinty function to see if DM is in vicinty and then uses GoldPickedUp to indicate that the gold has been found
private:
	bool found = false;
};

#endif // ACTOR_H_
