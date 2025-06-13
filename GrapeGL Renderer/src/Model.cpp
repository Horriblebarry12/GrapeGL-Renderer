#include "Model.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags

Model::Model(std::string filename)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (nullptr == scene)
	{
		//DoTheErrorLogging(importer.GetErrorString());
	}

	Meshes = std::vector<Mesh>();
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		Meshes.push_back(Mesh(scene->mMeshes[i]));
	}
}

void Model::Bind()
{
	for (Mesh mesh : Meshes)
	{
		mesh.Bind();
	}
}
