#pragma once
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <string>

struct Vertex
{
public:
	Vertex();
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord);

	void SetPosition(glm::vec3 position);
	void SetNormal(glm::vec3 normal);
	void SetTexCoord(glm::vec2 texCoord);

	glm::vec3 GetPosition() const;
	glm::vec3 GetNormal() const;
	glm::vec2 GetTexCoord() const;

	std::vector<float> toArray();
private:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

