#include "GeometryHelper.h"

#include <cassert>

Mesh GeometryHelper::CreatePlane(size_t xSlices, size_t ySlices, std::function<float(float, float)> heightFunc)
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
		v.y = heightFunc(posX, posZ);
		v.z = posZ;

		vertices.push_back(v);

		posX += xStep;
		if ((i + 1) % xSlices == 0)
		{
			posZ -= yStep;
			posX = -0.5f;
		}
	}

	for (size_t i = 0; i < xSlices * ySlices; i++)
	{
		// tangent calculation
		Vector3 tangent;
		float factor = 0;
		if (i % xSlices != 0)
		{
			tangent += (vertices[i - 1] - vertices[i]);
			factor++;
		}
		if ((i + 1) % xSlices != 0)
		{
			tangent += (vertices[i] - vertices[i + 1]);
			factor++;
		}
		assert(factor != 0);
		tangent = { tangent.x / factor, tangent.y / factor, tangent.z / factor };


		// bi-tangent calculation
		Vector3 bitangent;
		factor = 0;
		if (i >= xSlices)
		{
			bitangent += (vertices[i - xSlices] - vertices[i]);
			factor++;
		}
		if (i + xSlices < xSlices * ySlices)
		{
			bitangent += (vertices[i] - vertices[i + xSlices]);
			factor++;
		}
		assert(factor != 0);
		bitangent = { bitangent.x / factor, bitangent.y / factor, bitangent.z / factor };

		// normal calculation
		Vector3 normal = tangent.cross(bitangent).normalised();
		normals.push_back(normal);
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
