#include "GeometryHelper.h"

Mesh GeometryHelper::CreatePlane(size_t xSlices, size_t ySlices)
{
	Mesh newMesh;

	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<int> indices;


	float xStep = 1.0f / (xSlices - 1);
	float yStep = 1.0f / (ySlices - 1);

	float posX = -0.5f;
	float posZ = 0.5f;

	for (size_t i = 0; i < xSlices * ySlices; i++)
	{
		Vector3 v;
		v.x = posX;
		v.y = 0;
		v.z = posZ;

		vertices.push_back(v);
		normals.push_back({ 0, 1, 0 });

		posX += xStep;
		if ((i + 1) % xSlices == 0)
		{
			posZ -= yStep;
			posX = -0.5f;
		}
	}

	int currentIndex = 0;
	for (size_t y = 0; y < ySlices - 1; y++)
	{
		for (size_t x = 0; x < xSlices - 1; x++)
		{
			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + xSlices);

			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + xSlices + 1);
			indices.push_back(currentIndex + xSlices);

			currentIndex++;
		}
		currentIndex++;
	}

	newMesh.setVertices(vertices);
	newMesh.setNormals(normals);
	newMesh.setIndices(indices);

	return newMesh;
}
