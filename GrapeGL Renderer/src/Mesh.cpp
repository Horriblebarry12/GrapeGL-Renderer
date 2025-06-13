#include "Mesh.h"
#include "glad/glad.h"
#include "Common.h"

glm::vec3 aiVec3ToGLMVec3(aiVector3D vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

Mesh::Mesh(std::vector<Vertex> verticies, std::vector<unsigned int> triangles)
{
	Verticies = verticies;
	Triangles = triangles;
}

Mesh::Mesh(aiMesh* mesh)
{
	Verticies = std::vector<Vertex>();
	Triangles = std::vector<unsigned int>();
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position = aiVec3ToGLMVec3(mesh->mVertices[i]);
		glm::vec3 normal = aiVec3ToGLMVec3(mesh->mNormals[i]);
		glm::vec2 texCoord = glm::vec2(0.0f);
		if (mesh->mTextureCoords[0])
			texCoord = glm::vec2(aiVec3ToGLMVec3(mesh->mTextureCoords[0][i]).x, aiVec3ToGLMVec3(mesh->mTextureCoords[0][i]).y);
		Verticies.push_back(Vertex(position, normal, texCoord));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			Triangles.push_back(face.mIndices[j]);
	}

	glGenVertexArrays(1, &ArrayID);

	unsigned int dataSize = sizeof(Vertex) * Verticies.size(); //sizeof(Vertex) * Verticies.size();
	glGenBuffers(1, &VertexBuffer);
	glBindVertexArray(ArrayID);


	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, dataSize, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, &Verticies[0]);

	GLint size = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	if (dataSize != size)
	{
		glDeleteBuffers(1, &VertexBuffer);
		// Log the error
		return;
	}
	glGenBuffers(1, &ElementID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementID);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Triangles.size() * sizeof(unsigned int), &Triangles[0], GL_STATIC_DRAW);
}

void Mesh::Bind()
{
	glBindVertexArray(ArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementID);
}
