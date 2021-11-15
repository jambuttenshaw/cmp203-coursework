#include "GeometryHelper.h"

#include "PerlinNoise.h"

#include <cassert>
#include <array>
#include <fstream>

#include <unordered_map>


// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#pragma warning(disable : 4996)


Mesh GeometryHelper::CreatePlane(size_t xSlices, size_t ySlices, Vector3 up, float uScale, float vScale, std::function<float(float, float)> heightFunc, Vector3 tangent, Vector3 bitangent)
{
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<unsigned int> indices;

	if (tangent.equals(Vector3::zero) || bitangent.equals(Vector3::zero))
	{
		tangent = { up.z, up.x, up.y };
		bitangent = tangent.cross(up);
	}


	float xStep = 1.0f / (xSlices - 1);
	float yStep = 1.0f / (ySlices - 1);

	float posX = -0.5f;
	float posY = 0.5f;

	for (size_t y = 0; y < ySlices; y++)
	{
		for (size_t x = 0; x < xSlices; x++)
		{
			Vector3 v{ tangent * posX + bitangent * posY + up * heightFunc(posX, posY) };

			vertices.push_back(v);
			texCoords.push_back({
					x * xStep * uScale,
					y * yStep * vScale
				});

			posX += xStep;
		}
		posY -= yStep;
		posX = -0.5f;
	}

	// recalculation of tangent and bitangent are required as the surface may no longer be flat
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

	unsigned int currentIndex = 0;
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

	return Mesh{
		vertices,
		normals,
		texCoords,
		indices
	};
}

Mesh GeometryHelper::CreateUnitCube(size_t resolution)
{
	Mesh unitCube;
	std::array<Vector3, 6> faceDirections =
	{
		Vector3{  1,  0,  0 },
		Vector3{ -1,  0,  0 },
		Vector3{  0,  1,  0 },
		Vector3{  0, -1,  0 },
		Vector3{  0,  0,  1 },
		Vector3{  0,  0, -1 }
	};
	for (const auto& dir : faceDirections)
	{
		Mesh face = CreatePlane(resolution, resolution, dir, 1.0f, 1.0f, [](float x, float z)->float { return 0.5f; });
		CombineMeshes(unitCube, face);
	}
	return unitCube;
}

Mesh GeometryHelper::CreateUnitDisc(size_t resolution)
{
	float deltaAngle = 2 * 3.1415926f / resolution;

	std::vector<Vector3> positions(resolution + 1);
	std::vector<Vector3> normals(resolution + 1, { 0, 1, 0 });
	std::vector<Vector2> texCoords(resolution + 1);
	std::vector<unsigned int> triangles(3 * resolution);

	// one central vertex
	positions[0] = Vector3::zero;
	texCoords[0] = { 0.5f, 0.5f };

	float angle = 0.0f;
	size_t triIndex = 0;
	for (size_t i = 0; i < resolution; i++)
	{
		positions[i + 1] = { cosf(angle), 0, sinf(angle) };
		texCoords[i + 1] = { 0.5f * (cosf(angle) + 1), 0.5f * (sinf(angle) + 1) };
		angle += deltaAngle;

		triangles[triIndex] = 0;
		triangles[triIndex + 1] = i + 1;
		if (i == 0)
			triangles[triIndex + 2] = resolution;
		else
			triangles[triIndex + 2] = i;
		triIndex += 3;
	}

	return Mesh{
		positions,
		normals,
		texCoords,
		triangles
	};
}

Mesh GeometryHelper::LoadObj(const std::string& filename)
{
	// these arrays may not be the same size after loading from the file!
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	
	// map from an obj format index (v/n/t) to a constructed Vertex object inside the vertices vector
	std::unordered_map<std::string, unsigned int> indexMap;

	// constructed model data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;


	FILE* file = fopen(filename.c_str(), "r");
	if (file == NULL)
	{
		assert(false && "Failed to open file");
		std::abort();
	}
	while (true)
	{
		char lineHeader[128];

		// Read first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 pos;
				int matches = fscanf(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
				assert((matches == 3) && "Failed to parse model");
				positions.push_back(pos);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector2 uv;
				int matches = fscanf(file, "%f %f\n", &uv.x, &uv.y);
				assert((matches == 2) && "Failed to parse model");
				uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				int matches = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				assert((matches == 3) && "Failed to parse model");
				normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",	&face[0], &face[1], &face[2],
																			&face[3], &face[4], &face[5],
																			&face[6], &face[7], &face[8]);
				if (matches != 9)
				{
					assert(false && "Failed to parse model");
					std::abort();
				}

				// loop through the 3 vertices
				for (size_t i = 0; i < 3; i++)
				{
					// construct a string to identify the pos, normal and uv that we require
					std::string id = std::to_string(face[i]) + "/" + std::to_string(face[i + 1]) + "/" + std::to_string(face[i + 2]);
					// find out if we have seen this pattern before
					if (indexMap.find(id) != indexMap.end())
					{
						// we have!
						// add the index of this vertex to the index array
						indices.push_back(indexMap[id]);
					}
					else
					{
						// we have not seen this vertex before,
						// so we must construct the vertex
						Vertex newVertex;

						// this relies on all positions, texcoords and normals being BEFORE face data in the file
						newVertex.Position = positions[face[i]];
						newVertex.TexCoord = uvs[face[i + 1]];
						newVertex.Normal = normals[face[i + 2]];

						unsigned int newIndex = static_cast<unsigned int>(vertices.size());
						vertices.push_back(newVertex);
						indices.push_back(newIndex);

						// log this vertex in the map, so if we see it again we know where to find it
						indexMap[id] = newIndex;
					}
				}
			}
		}
	}

	fclose(file);

	// return a mesh constructed using the file data
	return Mesh(vertices, indices);
}

void GeometryHelper::CombineMeshes(Mesh& a, Mesh& b)
{
	// offset the indices in b by how many vertices are already in a
	size_t aVertexCount = a.Vertices.size();
	for (auto& index : b.Indices) index += aVertexCount;

	a.Vertices.insert(a.Vertices.end(), b.Vertices.begin(), b.Vertices.end());

	a.Indices.insert(a.Indices.end(), b.Indices.begin(), b.Indices.end());
}


#pragma region HeightFuncs

float GeometryHelper::HeightFuncs::PerlinNoiseTerrain(float x, float z)
{
	static PerlinNoise noise;

	float value = 0.0f;
	float s = 1.2f;
	float f = 4;
	for (int i = 0; i < 4; i++)
	{
		value += s * (float)noise.noise(f * (double)x, 0, f * (double)z);
		f *= 1.7f;
		s *= 0.5f;
	}

	return 0.7f * value - 1.0f;
}

#pragma endregion