#pragma once
#include "glm/glm.hpp"

struct Vertex
{
	// Attributes
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texCoord; // UV
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec4 bitangent;;
	
};