/**
 * @file opengl-template/Mesh.cpp
 *
 * OpenGL Template
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 *
 * OpenGL Template is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <opengl-template/Mesh.h>

using namespace Assimp;
using namespace OpenGLTemplate;
using namespace std;

// Public

void Mesh::draw() const
{
	_effect->apply();
	_bufferState.apply();
	applyTextureMaps();
	glDrawElements(GL_TRIANGLES, _indexBuffer.elementCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	unapplyTextureMaps();
	_bufferState.unapply();
	_effect->unapply();
}



// Static

shared_ptr<Mesh> Mesh::load(const string& filepath, const shared_ptr<Effect>& effect,
	const TextureMaps& textureMaps)
{
	Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

	const aiScene* scene = importer.ReadFile(filepath,
		aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_Triangulate);

	assert(scene != nullptr);
	assert(scene->HasMeshes());
	aiMesh* mesh = scene->mMeshes[0];
	const bool meshHasTextureCoords = mesh->HasTextureCoords(0u);
	const bool meshHasNormals = mesh->HasNormals();
	const bool meshHasTangentsAndBitangents = mesh->HasTangentsAndBitangents();

	unsigned int vertexSize = 3u;
	VertexComponents vertexComponents = VertexComponents::Positions;

	if(meshHasTextureCoords)
	{
		vertexSize += 2u;
		vertexComponents |= VertexComponents::TextureCoordinates;
	}

	if(meshHasNormals)
	{
		vertexSize += 3u;
		vertexComponents |= VertexComponents::Normals;
	}

	if(meshHasTangentsAndBitangents)
	{
		vertexSize += 6u;
		vertexComponents |= VertexComponents::TangentsAndBitangents;
	}

	vector<float>pos(3u * mesh->mNumVertices);

	vector<float> vertices(vertexSize * mesh->mNumVertices);
	vector<unsigned int> indices;
	indices.reserve(3u * mesh->mNumFaces);
	
	for(unsigned int i = 0u; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];

		for(unsigned int j = 0u; j < 3u; ++j)
		{
			const unsigned int index = face.mIndices[j];
			indices.push_back(index);

			//TÄÄLLÄ ON VIRHE
			// MIKSET JO KORJAA

			float x = mesh->mVertices[index].x;
			float y = mesh->mVertices[index].y;
			float z = mesh->mVertices[index].z;

			vertices[vertexSize * index]	  = mesh->mVertices[index].x;
			vertices[vertexSize * index + 1u] = mesh->mVertices[index].y;
			vertices[vertexSize * index + 2u] = mesh->mVertices[index].z;
			unsigned int offset = 3u;
		
			pos[3u * index]		 = mesh->mVertices[index].x;
			pos[3u * index + 1u] = mesh->mVertices[index].y;
			pos[3u * index + 2u] = mesh->mVertices[index].z;


			if(meshHasTextureCoords)
			{
				vertices[vertexSize * index + offset]	   = mesh->mTextureCoords[0][index].x;
				vertices[vertexSize * index + offset + 1u] = mesh->mTextureCoords[0][index].y;
				offset += 2u;
			}

			if(meshHasNormals)
			{
				vertices[vertexSize * index + offset]	   = mesh->mNormals[index].x;
				vertices[vertexSize * index + offset + 1u] = mesh->mNormals[index].y;
				vertices[vertexSize * index + offset + 2u] = mesh->mNormals[index].z;
				offset += 3u;
			}

			if(meshHasTangentsAndBitangents)
			{
				vertices[vertexSize * index + offset]	   = mesh->mTangents[index].x;
				vertices[vertexSize * index + offset + 1u] = mesh->mTangents[index].y;
				vertices[vertexSize * index + offset + 2u] = mesh->mTangents[index].z;
				vertices[vertexSize * index + offset + 3u] = mesh->mBitangents[index].x;
				vertices[vertexSize * index + offset + 4u] = mesh->mBitangents[index].y;
				vertices[vertexSize * index + offset + 5u] = mesh->mBitangents[index].z;
			}
		}
	}

	Mesh* ogltMesh = new Mesh(vertices, indices, vertexComponents, effect, textureMaps);
	// TODO: add 'loaded' log entry
	
	ogltMesh->addPosVertices(pos, indices);
	return shared_ptr<Mesh>(ogltMesh);
}

void Mesh::addPosVertices(std::vector<float>pos, std::vector<unsigned int> ind)
{
	posVertices = pos;
	posIndices = ind;
}

std::vector<float> Mesh::getVertices()
{
	return posVertices;
}

std::vector<unsigned int> Mesh::getIndices()
{
	return posIndices;
}
// Private

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
	const VertexComponents& vertexComponents, const std::shared_ptr<Effect>& effect, const TextureMaps& textureMaps)
	: _vertexBuffer(vertices.size(), sizeof(float), BufferType::Vertex, BufferUsage::DrawStatic),
	  _indexBuffer(indices.size(), sizeof(unsigned int), BufferType::Index, BufferUsage::DrawStatic),
	  _effect(effect),
	  _diffuseMap(textureMaps.diffuse),
	  _normalMap(textureMaps.normal),
	  _vertexComponents(vertexComponents)
{
	_vertexBuffer.setData(vertices);
	_indexBuffer.setData(indices);

	VertexFormat vertexFormat
	{
		{ 0u, 3u, VertexElementType::Float32, GL_FALSE }
	};

	if((_vertexComponents & VertexComponents::TextureCoordinates) == VertexComponents::TextureCoordinates)
		vertexFormat.push_back({ 1u, 2u, VertexElementType::Float32, GL_FALSE });

	if((_vertexComponents & VertexComponents::Normals) == VertexComponents::Normals)
		vertexFormat.push_back({ 2u, 3u, VertexElementType::Float32, GL_FALSE });

	if((_vertexComponents & VertexComponents::TangentsAndBitangents) == VertexComponents::TangentsAndBitangents)
	{
		vertexFormat.push_back({ 3u, 3u, VertexElementType::Float32, GL_FALSE });
		vertexFormat.push_back({ 4u, 3u, VertexElementType::Float32, GL_FALSE });
	}

	_bufferState.initialise(vertexFormat, &_vertexBuffer, &_indexBuffer);
}

void Mesh::applyTextureMaps() const
{
	if(_diffuseMap)
	{
		glActiveTexture(GL_TEXTURE0);
		_diffuseMap->apply();
	}

	if(_normalMap)
	{
		glActiveTexture(GL_TEXTURE0 + 1u);
		_normalMap->apply();
	}
}

void Mesh::unapplyTextureMaps() const
{
	if(_normalMap)
	{
		glActiveTexture(GL_TEXTURE0 + 1u);
		_normalMap->unapply();
	}

	if(_diffuseMap)
	{
		glActiveTexture(GL_TEXTURE0);
		_diffuseMap->unapply();
	}
}
