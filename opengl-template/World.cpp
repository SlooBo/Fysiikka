#include "World.h"
#include <time.h>

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
		obj.vertex[i].x += obj.velocity.x * deltaTime + gravity.x * (deltaTime * deltaTime) * 0.5;
		obj.vertex[i].y += obj.velocity.y * deltaTime + gravity.y * (deltaTime * deltaTime) * 0.5;
		obj.vertex[i].z += obj.velocity.z * deltaTime + gravity.z * (deltaTime * deltaTime) * 0.5;
	}
	obj.velocity.x += gravity.x * deltaTime;
}

void World::Acceleration(FObject obj, glm::vec3 acceleration)
{
	obj.velocity += acceleration * deltaTime;
}