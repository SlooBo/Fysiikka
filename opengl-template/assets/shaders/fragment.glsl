/**
 * @file assets/shaders/fragment.glsl
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

#version 330

in vec2 textureCoords;
in vec3 normal;
in vec3 viewDirection;
in vec3 lightDirection;

uniform sampler2D unifDiffuseSampler;

out vec4 outColour;

const vec4 ambientColour = vec4(0.05, 0.05, 0.05, 1.0);
const vec4 specularColour = vec4(0.4, 0.4, 0.4, 1.0);

void main()
{
	float diffuseIntensity = dot(normal, lightDirection);
	diffuseIntensity = max(0.0, diffuseIntensity);
	float specularIntensity = 0.0;

	if(diffuseIntensity > 0.0)
	{
		vec3 halfDirection = normalize(lightDirection + viewDirection);
		float specularAngle = dot(normal, halfDirection);
		specularAngle = max(0.0, specularAngle);
		specularIntensity = pow(specularAngle, 16.0);
	}

	vec4 diffuseColour = texture(unifDiffuseSampler, textureCoords);
	outColour = ambientColour + diffuseIntensity * diffuseColour + specularIntensity * specularColour;
}