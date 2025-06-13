#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord)
{
	Position = position;
	Normal = normal;
	TexCoord = texCoord;
}

void Vertex::SetPosition(glm::vec3 position)
{
	Position = position;
}

void Vertex::SetNormal(glm::vec3 normal)
{
	Normal = normal;
}

void Vertex::SetTexCoord(glm::vec2 texCoord)
{
	TexCoord = texCoord;
}

glm::vec3 Vertex::GetPosition() const
{
	return Position;
}

glm::vec3 Vertex::GetNormal() const
{
	return Normal;
}

glm::vec2 Vertex::GetTexCoord() const
{
	return TexCoord;
}

std::vector<float> Vertex::toArray()
{
	return { Position.x, Position.y, Position.z, 
			 Normal.x, Normal.y, Normal.z, 
			 TexCoord.x, TexCoord.y };
}

