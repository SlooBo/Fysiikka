#include "opengl-template\FObject.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace OpenGLTemplate;

FObject::FObject(const FObject &)
{

}
void FObject::Draw()
{
	_effect->apply();
	_bufferState.apply();
	
	glDrawElements(GL_TRIANGLES, _indexBuffer.elementCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

	_bufferState.unapply();
	_effect->unapply();

}

std::shared_ptr<FObject> FObject::CreateFObject(const std::shared_ptr<Effect>& effect, Mesh& _mesh)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	////TODO IMPORTANT SHIT
	/*
		jos tarvitaan muuta meshi dataa jatkossa niin hae se t‰‰ll‰ ja p‰ivit‰ constructoria...
	*/
	
	FObject* obj = new FObject(_mesh.getVertices(), _mesh.getIndices(), effect);
	return std::shared_ptr<FObject>(obj);
}
//Dont use this one.
FObject::FObject(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Effect>& effect)
	:_vertexBuffer(vertices.size(), sizeof(float), BufferType::Vertex, BufferUsage::DrawDynamic),
	_indexBuffer(indices.size(), sizeof(unsigned int), BufferType::Index, BufferUsage::DrawDynamic),
	_effect(effect)
{

	//Create data for drawing.
	_vertexBuffer.setData(vertices);
	_indexBuffer.setData(indices);
	
	eStart = 0;
	averageDistance = 0;
	numberEdges = 0;
	k = 50;
	fixed = false;

	VertexFormat vertexFormat
	{
		{ 0u, 3u, VertexElementType::Float32, GL_FALSE }
	};
	_bufferState.initialise(vertexFormat, &_vertexBuffer, &_indexBuffer);

	int numberVertices = vertices.size();
	int numberIndices = indices.size();
	int numberFaces = numberIndices / 3;

	eSize = numberFaces * 3 / 2;

	edges = new Edge[eSize];

	for (int i = 0; i < vertices.size() / 3; i++)
	{
		Vertex v(vertices[i*3], vertices[i*3+1], vertices[i*3+2]);
		AddVertex(v, Vector(0, 0, 0) );
	}

	for (int i = 0; i < indices.size() / 3; i++)
	{
		AddFace(indices[i * 3], indices[i * 3 + 1], indices[i * 3 + 2]);
	}

	///// TODO IMPORTANT SHIT
	/*
		construct the real FObject here with the own functions
		use the data from vertices and indices
	
		you know what to do... GO!
	
		Mik‰‰n ei toimi koskaan miss‰‰n ennenkuin t‰‰ on tehty


		Pit‰‰ lis‰t‰ viel updateen sitten se ett‰ kun siel muuttuu vertexi data niin...
		se sitten asetetaan myˆs buffereille uudestaan k‰ytt‰en tuota _vertexBuffer.setData()aaaa
	*/
	BuildDone();
	averageDistance = averageDistance / numberEdges;

	gravity = Vector(0, -9.81, 0);
	mass = vertex.size();

}

void FObject::AddVertex(Vertex v, Vector vel)
{
	NeighborList *neighbors = new NeighborList;
	vertex.push_back(v);
	oldPos.push_back(v);

	velocity.push_back(vel);
	oldVelocity.push_back(vel);
	force_external.push_back(Vector(0, 0, 0));
	force_internal.push_back(Vector(0, 0, 0));
	acceleration.push_back(Vector(0, 0, 0));
	neighbours.push_back(neighbors);

	dep1.push_back(Vector(0, 0, 0));
	dep2.push_back(Vector(0, 0, 0));
	dep3.push_back(Vector(0, 0, 0));
}

void FObject::AddEdge(int v1, int v2)
{
	Edge edge;

	edge.v1 = &vertex[v1];
	edge.v2 = &vertex[v2];

	if (!EdgeExists(edge))
	{
		edges[eStart] = edge;

		eStart++;
	}	
}


bool FObject::EdgeExists(Edge edge)
{
	for (int i = 0; i < eStart; i++)
	{
		if (edges[i] == edge)
		{
			return true;
		}
	}
	return false;
}
void FObject::AddFace(int v1, int v2, int v3)
{

	Polygon *polygon = new Polygon;
	polygon->vertex[0] = &vertex[v1];
	polygon->vertex[1] = &vertex[v2];
	polygon->vertex[2] = &vertex[v3];
	polygon->normal = polygon->Normal();

	AddEdge(v1, v2);
	AddEdge(v2, v3);
	AddEdge(v3, v1);

	face.push_back(polygon);

	neighbours[v1]->Add(&vertex[v2], &vertex[v1], v2);
	neighbours[v1]->Add(&vertex[v3], &vertex[v1], v3);
	Vector vector1 = vertex[v2] - vertex[v1];
	Vector vector2 = vertex[v3] - vertex[v1];
	
	averageDistance += ~v1;
	averageDistance += ~v2;
	numberEdges += 2;

	neighbours[v2]->Add(&vertex[v1], &vertex[v2], v1);
	neighbours[v2]->Add(&vertex[v3], &vertex[v2], v3);
	vector1 = vertex[v1] - vertex[v2];
	vector2 = vertex[v3] - vertex[v2];

	averageDistance += ~v1;
	averageDistance += ~v2;
	numberEdges += 2;

	neighbours[v3]->Add(&vertex[v1], &vertex[v3], v1);
	neighbours[v3]->Add(&vertex[v2], &vertex[v3], v2);
	vector1 = vertex[v1] - vertex[v3];
	vector2 = vertex[v2] - vertex[v3];

	averageDistance += ~v1;
	averageDistance += ~v2;
	numberEdges += 2;
}

void FObject::SetPosition(Vertex p)
{
	float xDiff = center.x - p.x;
	float yDiff = center.y - p.y;
	float zDiff = center.z - p.z;

	this->center = p;

	for (unsigned int i = 0; i < vertex.size(); i++) {
		vertex[i].x -= xDiff;
		vertex[i].y -= yDiff;
		vertex[i].z -= zDiff;
	}

}

void FObject::SetVelocity(int i, Vector v) {
	if (i == -1) {
		for (int j = 0; j < (int)velocity.size(); j++)
			velocity[j] = v;
	}
	else {
		if (i < (int)velocity.size())
			velocity[i] = v;
	}
}

void FObject::SetExAcceleration(int i, Vector v) {
	if (i < (int)force_external.size())
		force_external[i] = v;
}

void FObject::BuildDone() {
	float x = 0, y = 0, z = 0;

	for (int i = 0; i < (int)vertex.size(); i++) {
		x += vertex[i].x;
		y += vertex[i].y;
		z += vertex[i].z;
	}

	x = x / (float)vertex.size();
	y = y / (float)vertex.size();
	z = z / (float)vertex.size();

	center.x = x;
	center.y = y;
	center.z = z;

	cdRadius = 0;
	for (int i = 0; i < (int)vertex.size(); i++) {
		distanceToCenter.push_back(center.GetDistance(vertex[i]));
		if (cdRadius < center.GetDistance(vertex[i]))
			cdRadius = center.GetDistance(vertex[i]);
	}

}

void FObject::Update() {

	int i;

	float DELTA_TIME = (float)1 / 100;//(float)Time::GetTimeElapsedMS()/1000;

	Vector centerVel;
	Vector centerAcc;

	for (i = 0; i < (int)vertex.size(); i++) {

		centerVel = centerVel + velocity[i];
		centerAcc = centerAcc + acceleration[i];

		oldVelocity[i] = velocity[i];
		oldPos[i] = vertex[i];

		vertex[i].x = oldPos[i].x + velocity[i].x * DELTA_TIME +
			acceleration[i].x * DELTA_TIME * DELTA_TIME / 2;

		vertex[i].y = oldPos[i].y + velocity[i].y * DELTA_TIME +
			acceleration[i].y * DELTA_TIME * DELTA_TIME / 2;

		vertex[i].z = oldPos[i].z + velocity[i].z * DELTA_TIME +
			acceleration[i].z * DELTA_TIME * DELTA_TIME / 2;

		velocity[i].x = oldVelocity[i].x + acceleration[i].x * DELTA_TIME;
		velocity[i].y = oldVelocity[i].y + acceleration[i].y * DELTA_TIME;
		velocity[i].z = oldVelocity[i].z + acceleration[i].z * DELTA_TIME;
	}

	centerVel = centerVel / vertex.size();
	centerAcc = centerAcc / vertex.size();
	center.x = center.x + centerVel.x * DELTA_TIME +
		centerAcc.x * DELTA_TIME * DELTA_TIME / 2;
	center.y = center.y + centerVel.y * DELTA_TIME +
		centerAcc.y * DELTA_TIME * DELTA_TIME / 2;
	center.z = center.z + centerVel.z * DELTA_TIME +
		centerAcc.z * DELTA_TIME * DELTA_TIME / 2;

	for (i = 0; i < (int)vertex.size(); i++) {
		Vector totalF(0, 0, 0);
		for (int j = 0; j < (int)neighbours[i]->vertexes.size(); j++) {
			float delta = neighbours[i]->DeltaDistance(vertex[i], j);
			float ratio = neighbours[i]->RatioDistance(vertex[i], j);

			Vector deltaVel = velocity[i] - velocity[neighbours[i]->indexes[j]];
			Vector deltaPos = vertex[i] - vertex[neighbours[i]->indexes[j]];
			
			if (!(0.0001 > delta && delta > -0.0001)) {
				Vector direct(((neighbours[i]->vertexes[j]->x) - vertex[i].x),
					((neighbours[i]->vertexes[j]->y) - vertex[i].y),
					((neighbours[i]->vertexes[j]->z) - vertex[i].z));

				direct = direct.Normalize();

				float c = 1 * ((deltaPos*deltaPos) / ((~deltaPos)*(~deltaPos)));

				direct.x = 1 * k * delta * direct.x - c * deltaVel.x;
				direct.y = 1 * k * delta * direct.y - c * deltaVel.y;
				direct.z = 1 * k * delta * direct.z - c * deltaVel.z;

				totalF = totalF + direct;
			}
			
		}
		
		float delta = center.GetDistance(vertex[i]) - distanceToCenter[i];
		Vector deltaVel = velocity[i] - centerVel; // - velocity[depend[i]->index[j]];
		
		Vector deltaPos = vertex[i] - center;
		
		if (!(0.0001 > delta && delta > -0.0001)) {
		Vector direct(  ((center.x ) - vertex[i].x),
		((center.y ) - vertex[i].y),
		((center.z ) - vertex[i].z));

		deltaPos = deltaPos.Normalize();
		direct = deltaPos;
		float c = ((deltaPos*deltaPos) / ((~deltaPos)*(~deltaPos))) ;

		direct.x = 1 * k * delta * direct.x - c * deltaVel.x;
		direct.y = 1 * k * delta * direct.y - c * deltaVel.y;
		direct.z = 1 * k * delta * direct.z - c * deltaVel.z;

		dep1[i] = direct;

		totalF = totalF + direct;
		}
		
		force_internal[i].x = totalF.x;
		force_internal[i].y = totalF.y;
		force_internal[i].z = totalF.z;

		acceleration[i] = ((force_internal[i] + force_external[i]) / ((float)(mass / (float)vertex.size())))
			+ gravity;

		if (vertex[i].y >= 0 && velocity[i].y < 0) {
			velocity[i].y = -velocity[i].y / 2;
			velocity[i].x = velocity[i].x / 2;
			velocity[i].z = velocity[i].z / 2;

			vertex[i].y = 0;
			
			for (int sp = 0; sp < vertex.size(); sp++) {
			if (sp != i){
			velocity[sp].y = - velocity[sp].y/1.5;
			velocity[sp].x = velocity[sp].x/1.5;
			velocity[sp].z = velocity[sp].z/1.5;
			}
			}
			
		}
	}

	float x = 0, y = 0, z = 0;

	for (int i = 0; i < (int)vertex.size(); i++) {
		x += vertex[i].x;
		y += vertex[i].y;
		z += vertex[i].z;
	}

	x = x / (float)vertex.size();
	y = y / (float)vertex.size();
	z = z / (float)vertex.size();

	center.x = x;
	center.y = y;
	center.z = z;

	cdRadius = 0;
	for (int i = 0; i < (int)vertex.size(); i++) {
		distanceToCenter.push_back(center.GetDistance(vertex[i]));
		if (cdRadius < center.GetDistance(vertex[i]))
			cdRadius = center.GetDistance(vertex[i]);
	}

	//DO WORK'
	//OPETELKAA KOODAAN SAATANA
	//EI Tƒƒ NƒIN TOIMI

	std::vector<float>neekeri;
	for (int i = 0; i < vertex.size(); i++)
	{
		neekeri.push_back(vertex[i].x);
		neekeri.push_back(vertex[i].y);
		neekeri.push_back(vertex[i].z);
	}
	
	_vertexBuffer.setData(neekeri);
}

Vertex FObject::GetPosition() const { return this->center; }
float   FObject::GetCDRadius() const { return this->cdRadius; }
bool   FObject::GetFixed() const { return fixed; }

FObject::~FObject() {
	for (int i = 0; i < (int)face.size(); i++) {
		delete face[i];
	}
}

bool FObject::CollisionDetected(FObject &o, std::vector<CollisionEdge> &collisionEdges,
	std::vector<Vertex> &collisionPoints) const
{
	std::vector<CollisionEdge>::iterator result;

	Vector N, e1, e2, directionVector;
	Vertex pointToCheck;
	int i, j;
	float e1_p, e2_p, distance, T, angle;
	Vector normal;

	Vector edgeVector1, edgeVector2;
	CollisionEdge* cEdge;


	for (i = 0; i < eSize; i++)
	{
		if (!edges[i].outerEdge)
		{
			std::cout << "Skip!" << std::endl;
			continue;  // NEW
		}

		e1 = ((*edges[i].v1) - Vertex(0, 0, 0));
		e2 = ((*edges[i].v2) - Vertex(0, 0, 0));
		directionVector = (*edges[i].v2) - (*edges[i].v1);
		directionVector.Normalize();

		for (j = 0; j < (int)o.face.size(); j++)
		{
			N = o.face[j]->normal;
			distance = N * ((*o.face[j]->vertex[0]) - Vertex(0, 0, 0));
			distance = 0 - distance;
			e1_p = N * e1 + distance;
			e2_p = N * e2 + distance;

			// Check if we have Edge To Plane Collision
			if ((e1_p < 0.0f && e2_p > 0.0f) || (e1_p > 0.0f && e2_p < 0.0f))
			{
				T = -((N * e1) + distance) / (N * directionVector);

				pointToCheck.x = (edges[i].v1->x) + directionVector.x * T;
				pointToCheck.y = (edges[i].v1->y) + directionVector.y * T;
				pointToCheck.z = (edges[i].v1->z) + directionVector.z * T;

				// If the edge is against the edge of the plane then force a collision
				// detection on it.
				if (pointToCheck == *o.face[j]->vertex[0] ||
					pointToCheck == *o.face[j]->vertex[1] ||
					pointToCheck == *o.face[j]->vertex[2])
					angle = 360.0f;
				else
				{
					angle = 0;

					for (int l = 2, z = 0; z < 3; l = z++)
					{
						int new_angle = ((*(o.face[j]->vertex[z])) - pointToCheck).Angle(
							((*o.face[j]->vertex[l]) - pointToCheck));

						angle += new_angle;
					}
				}

				// Now check if we have Point to Polygon Collision 
				if (angle >= 355.0f)
				{
					cEdge = new CollisionEdge();

					// Find out which endpoint was inside the other object.
					// Incase of the possibility that the edge goes completely
					// through to the outside of the object, then we will mark
					// this edge as a "non interior" edge and completey ignore it
					// when running DFS.
					cEdge->vertex[0] = &(*edges[i].v1);
					cEdge->vertex[1] = &(*edges[i].v2);


					edgeVector1 = Vector(edges[i].v1->x - edges[i].v2->x,
						edges[i].v1->y - edges[i].v2->y,
						edges[i].v1->z - edges[i].v2->z);

					edgeVector2 = Vector(edges[i].v2->x - edges[i].v1->x,
						edges[i].v2->y - edges[i].v1->y,
						edges[i].v2->z - edges[i].v1->z);

					cEdge->collisionIdx = N.Angle(edgeVector1) > 90 ? 0 : 1;

					// If we have already added this edge into the set of collision
					// edges then it means the edge has left the object or has re-entered
					// the object.  In this case, then reverse the interior status of the
					// object so we know if the edge fully penetrates the object or not.
					if ((result = find(collisionEdges.begin(), collisionEdges.end(), *cEdge)) != collisionEdges.end())
						(*result).interior = !(*result).interior;
					else
					{
						// Remember this edge as part of the collision
						cEdge->interior = true;
						collisionEdges.push_back(*cEdge);
					}

					// Add the points of the triangle into the collision point set
					for (int i = 0; i < 3; i++)
					{
						if (find(collisionPoints.begin(), collisionPoints.end(),
							*o.face[j]->vertex[i]) == collisionPoints.end())
							collisionPoints.push_back(*o.face[j]->vertex[i]);
					}
				}
			}
		}
	}

	return collisionEdges.size() != 0;
}


void FObject::DFS(std::vector<CollisionEdge> &collisionEdges, std::vector<Vertex> &collisionPoints)
{
	std::vector<CollisionEdge>::iterator result;
	int i = 0;

	// Mark all edges as unprocessed.
	for (i = 0; i < eSize; i++)
	{
		edges[i].state = Edge::states::None;

		if (find(collisionEdges.begin(), collisionEdges.end(), edges[i]) != collisionEdges.end())
			edges[i].state = Edge::states::Done;
	}

	for (i = 0; i < (int)collisionEdges.size(); i++)
	{
		if (collisionEdges[i].interior)
		{
			collisionPoints.push_back(*collisionEdges[i].vertex[(collisionEdges[i].collisionIdx + 1) % 2]);
			DFSRec(*collisionEdges[i].vertex[collisionEdges[i].collisionIdx], collisionPoints);
		}
	}
}

void FObject::DFSRec(Vertex &p, std::vector<Vertex> &collisionPoints)
{
	for (int i = 0; i < eSize; i++)
	{
		if ((*edges[i].v1 == p || *edges[i].v2 == p) && edges[i].state == Edge::states::None)
		{
			edges[i].state = Edge::states::Processing;

			if (*edges[i].v1 == p)
			{
				DFSRec(*edges[i].v2, collisionPoints);
				collisionPoints.push_back(*edges[i].v2);
			}
			else
			{
				DFSRec(*edges[i].v1, collisionPoints);
				collisionPoints.push_back(*edges[i].v1);
			}

			edges[i].state = Edge::states::Done;
		}
	}
}


bool Edge::operator ==(const Edge &e)
{
	return ((e.v1->x == v1->x) && (e.v1->y == v1->y) && (e.v1->z == v1->z) &&
		(e.v2->x == v2->x) && (e.v2->y == v2->y) && (e.v2->z == v2->z)) ||
		((e.v1->x == v2->x) && (e.v1->y == v2->y) && (e.v1->z == v2->z) &&
		(e.v2->x == v1->x) && (e.v2->y == v1->y) && (e.v2->z == v1->z));
}

bool Edge::operator ==(const CollisionEdge &e)
{
	return (e.vertex[0]->x == v1->x) && (e.vertex[0]->y == v1->y) && (e.vertex[0]->z == v1->z) &&
		(e.vertex[1]->x == v2->x) && (e.vertex[1]->y == v2->y) && (e.vertex[1]->z == v2->z);
}

// NEW
bool CollisionEdge::operator==(const CollisionEdge &e)
{
	return (e.vertex[0]->x == vertex[0]->x) && (e.vertex[0]->y == vertex[0]->y) && (e.vertex[0]->z == vertex[0]->z) &&
		(e.vertex[1]->x == vertex[1]->x) && (e.vertex[1]->y == vertex[1]->y) && (e.vertex[1]->z == vertex[1]->z);
}

// NEW
bool CollisionEdge::operator==(const Edge &e)
{
	return (e.v1->x == vertex[0]->x) && (e.v1->y == vertex[0]->y) && (e.v1->z == vertex[0]->z) &&
		(e.v2->x == vertex[1]->x) && (e.v2->y == vertex[1]->y) && (e.v2->z == vertex[1]->z);
}
