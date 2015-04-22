#include "opengl-template\World.h"
#include "opengl-template\Vector.h"
#include "opengl-template\Vertex.h"

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

void World::DoWork()
{
	std::vector<CollisionEdge> collisionEdgesA, collisionEdgesB;
	std::vector<Vertex> collisionPointA, collisionPointB;

	unsigned int i, n;
	float rad, dist;
	calcDeltaTime();

	if ((time_elapsed = deltaTime) >= 4)
	{

		for (i = 0; i < objects.size(); i++)
		{
			//TODO
			//objects[i]->Update();
		}

		for (i = 0; i < objects.size(); i++)
			for (n = 0; n < objects.size(); n++)
			{
			Vertex k = objects[i]->GetPosition();
			Vertex j = objects[n]->GetPosition();

			if (i != n && (dist = objects[i]->GetPosition().GetDistance(objects[n]->GetPosition())) <=
				(rad = objects[i]->GetCDRadius() + objects[n]->GetCDRadius()))
			{
				// Do only if one of the objects is not fixed
				if (!objects[i]->GetFixed() || !objects[n]->GetFixed())
				{

					if (objects[i]->CollisionDetected(*objects[n], collisionEdgesA, collisionPointB))
					{
						objects[n]->CollisionDetected(*objects[i], collisionEdgesB, collisionPointA);

						objects[i]->DFS(collisionEdgesA, collisionPointA);
						objects[n]->DFS(collisionEdgesB, collisionPointB);


						//This code makes the edges and polygons of intersection visible
						//on the screen, this is just for debuggging purposes.
						// cout << collisionPointA.size() << endl;
						// cout << collisionPointB.size() << endl;

						//int x = 0;
						/*
						for( x=0; x < collisionPointA.size(); x++ )
						addGlutSphere(collisionPointA[x] , .2);
						for( x=0; x < collisionPointB.size(); x++ )
						addGlutSphere(collisionPointB[x] , .2);
						*/

						// Collision Response.

						//cout << "Colided!!! " << (int)collisionPointA.size() << endl;

						std::vector<int> pointIndexI, pointIndexII;

						for (int pi = 0; pi < (int)objects[i]->vertex.size(); pi++) {
							for (int x = 0; x < (int)collisionPointA.size(); x++){
								if (collisionPointA[x] == objects[i]->vertex[pi]){
									pointIndexI.push_back(pi);
									break;
									//cout <<x << "P: " << pi << endl;                     
								}
							}
						}


						for (int pi = 0; pi < (int)objects[n]->vertex.size(); pi++) {
							for (int x = 0; x < (int)collisionPointB.size(); x++){
								if (collisionPointB[x] == objects[n]->vertex[pi]){
									pointIndexII.push_back(pi);
									break;
									//cout <<x << "P: " << pi << endl;                     
								}
							}
						}
						//cout << pointIndexI.size() << " " << pointIndexII.size() << endl;
						float avgVelIz = 0, avgVelIIz = 0;

						for (int k = 0; k < (int)pointIndexI.size(); k++) {
							avgVelIz += objects[i]->velocity[k].z;

						}
						avgVelIz = avgVelIz / pointIndexI.size();

						for (int k = 0; k < (int)pointIndexII.size(); k++) {
							avgVelIIz += objects[n]->velocity[k].z;
						}

						avgVelIIz = avgVelIIz / pointIndexII.size();

						float newVelIz, newVelIIz;

						newVelIz = -avgVelIz / 2;
						newVelIIz = -avgVelIIz / 2;


						//--------------
						float avgVelIx = 0, avgVelIIx = 0;

						for (int k = 0; k < (int)pointIndexI.size(); k++) {
							avgVelIx += objects[i]->velocity[k].x;
						}
						avgVelIx = avgVelIx / pointIndexI.size();

						for (int k = 0; k < (int)pointIndexII.size(); k++) {
							avgVelIIx += objects[n]->velocity[k].x;
						}

						avgVelIIx = avgVelIIx / pointIndexII.size();

						float newVelIx, newVelIIx;

						newVelIx = -avgVelIx / 2;
						newVelIIx = -avgVelIIx / 2;

						//-------------------
						float avgVelIy = 0, avgVelIIy = 0;

						for (int k = 0; k < (int)pointIndexI.size(); k++) {
							avgVelIy += objects[i]->velocity[k].y;
						}
						avgVelIy = avgVelIy / pointIndexI.size();

						for (int k = 0; k < (int)pointIndexII.size(); k++) {
							avgVelIIy += objects[n]->velocity[k].y;
						}

						avgVelIIy = avgVelIIy / pointIndexII.size();

						float newVelIy, newVelIIy;

						newVelIy = avgVelIIy;
						newVelIIy = avgVelIy;

						//-----------
						//cout << newVelIx << ", " << newVelIy << ", " << newVelIz << endl;
						//cout << newVelIIx << ", " << newVelIIy << ", " << newVelIIz << endl;
						//cout << "--------------------------" << endl;

						for (int k = 0; k < (int)pointIndexI.size(); k++) {
							objects[i]->SetVelocity(k, Vector(/*0,0,0*/newVelIx, newVelIy, newVelIz));
							objects[i]->vertex[k] = objects[i]->oldPos[k];
						}

						for (int k = 0; k < (int)pointIndexII.size(); k++) {
							objects[n]->SetVelocity(k, Vector(/*0,0,0*/newVelIIx, newVelIIy, newVelIIz));
							objects[n]->vertex[k] = objects[n]->oldPos[k];
						}

					}
				}
			}
			}
	}
}