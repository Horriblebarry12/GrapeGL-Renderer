#pragma once
#include "Mesh.h"

class Model
{
public:
	std::vector<Mesh> Meshes;

	Model(std::string filename);

	void Bind();

};

