#pragma once
#include "glm/glm.hpp"

struct Vertex
{
	// Attribute
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texCoord; // UV
	glm::vec4 normal;
};