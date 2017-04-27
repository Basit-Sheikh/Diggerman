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
	void clearDirt();
};
class Boulder : public Actor {

};
class Protester : public Actor {

};
class HardcoreProtester : public Protester {

};


#endif // ACTOR_H_
