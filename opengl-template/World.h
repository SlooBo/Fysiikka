#pragma once
#include <vector>
#include "FObject.h"

class World
{
public:
	World();
	~World();

	void AddObject(FObject *obj);
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
	const glm::vec3 gravity = glm::vec3(0, -9.81, 0);
	std::vector<FObject*> objects;
};
