#include "opengl-template\World.h"

World::World()
{
	t = clock();
}


World::~World(){}


void World::AddObject(FObject *obj)
{
	objects.push_back(obj);

}
void World::Update()
{
	//DeltaTime start
	calcDeltaTime();

	/// Calculate forces for each object.





	//Move all the objects after the forces have been calculated.
	for (int i = 0; i < objects.size(); i++)
		Movement(*objects[i]);


	//Draw everything
	//for (int i = 0; i < objects.size(); i++)
	//	Draw(*objects[i]);

}

void World::calcDeltaTime()
{
	//deltaTime finish
	t = clock() - t;

	deltaTime = ((float)t) / CLOCKS_PER_SEC;

	t = clock();
}

void World::Movement(FObject obj)
{
	for (int i = 0; i < obj.vertex.size(); i++)
	{
		obj.oldPos[i] = obj.vertex[i];

		// newPos = currentPos + Velocity * Time + 1/2 * Acceleration * Time^2
		obj.vertex[i].x += obj.velocity[0].x * deltaTime + gravity.x * (deltaTime * deltaTime) * 0.5;
		obj.vertex[i].y += obj.velocity[0].y * deltaTime + gravity.y * (deltaTime * deltaTime) * 0.5;
		obj.vertex[i].z += obj.velocity[0].z * deltaTime + gravity.z * (deltaTime * deltaTime) * 0.5;
	}
	obj.velocity[0].x += gravity.x * deltaTime;
}

void World::Acceleration(FObject obj, Vector acceleration)
{
	obj.velocity[0] += acceleration * deltaTime;
}