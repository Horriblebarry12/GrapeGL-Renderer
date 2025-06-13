#pragma once
#include "Vertex.h"
#include <assimp/scene.h>           // Output data structure


class Mesh
{
public:

	Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> triangles);
	Mesh(aiMesh* mesh);

	std::vector<Vertex> Verticies;
	std::vector<unsigned int> Triangles;

	void Bind();

private:

	unsigned int ArrayID;
	unsigned int VertexBuffer;
	unsigned int ElementID;
};

