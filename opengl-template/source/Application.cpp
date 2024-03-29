/**
 * @file opengl-template/Application.cpp
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
#include <vector>
#include <opengl-template/Application.h>
#include <opengl-template/EffectSource.h>

using namespace glm;
using namespace OpenGLTemplate;
using namespace std;

// Public

Application::Application()
	: _worldTransformLocation(0u),
	  _rotation(0.0f)
{
	// Initialisation

	EffectSource effectSource;
	effectSource.load(ShaderType::Vertex, "assets/shaders/vertex.glsl");
	effectSource.load(ShaderType::Fragment, "assets/shaders/fragment.glsl");
	_effect.reset(new Effect(effectSource));
	_effect->apply();

	mat4 transform = perspective(radians(90.0f),
		static_cast<GLfloat>(Config::WINDOW_WIDTH) / Config::WINDOW_HEIGHT, 0.01f, 1000.0f);

	GLint location = _effect->getUniformLocation("unifProjectionTransform");
	assert(location != -1);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));

	transform = translate(vec3(0.0f, 0.0f, -10.0f));
	location = _effect->getUniformLocation("unifViewTransform");
	assert(location != -1);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));

	location = _effect->getUniformLocation("unifWorldTransform");
	assert(location != -1);
	_worldTransformLocation = location;
	glUniformMatrix4fv(_worldTransformLocation, 1, GL_FALSE, value_ptr(mat4()));
	_effect->unapply();

	TextureMaps textureMaps;

	// Add a texture to the path below
	textureMaps.diffuse = Texture::load(TextureType::Texture2D, "assets/textures/texture.png");

	// Add a model file to the path below
	_mesh = Mesh::load("assets/models/cube.dae", _effect, textureMaps);

	std::shared_ptr<FObject> fobj = FObject::CreateFObject(_effect, *_mesh);
	fobj->SetPosition(Vertex(0, -7, 0));
	fobj->gravity = Vector(0, 0, 0);
	fobj->SetExAcceleration(1, Vector(0,5,0));
	world.AddObject(fobj);

	std::shared_ptr<FObject> fobj2 = FObject::CreateFObject(_effect, *_mesh);
	fobj2->SetPosition(Vertex(0.5, -1, 0));
	fobj2->gravity = Vector(0, -9.81, 0);
	world.AddObject(fobj2);

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

}

Application::~Application()
{
	// Deinitialisation
}

void Application::update()
{
	// Updating and drawing

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_effect->apply();
	_rotation += 0.005f;
	const mat4 worldTransform = rotate(_rotation, vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(_worldTransformLocation, 1, GL_FALSE, value_ptr(worldTransform));

	world.Update();
}
