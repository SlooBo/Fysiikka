#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include "FObject.h"
#include <time.h>

class World
{
public:
	World();
	~World();

	void AddObject(	std::shared_ptr<FObject> _fobject);
	void DoWork();
	void Update();

private:
	//Calculate the delta time since last call.
	void calcDeltaTime();
	//Calculates the movement with gravity and previous forces.
	void Movement(FObject obj);
	//Calculates the new velocity for the object.
	void Acceleration(FObject obj, Vector acceleration);


	clock_t t;
	float deltaTime;
	float time_elapsed;
	const glm::vec3 gravity = glm::vec3(0, -9.81, 0);
	std::vector<std::shared_ptr<FObject>> objects;
};
#endif
