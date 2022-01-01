#include "GeometryHelper.h"

#include "PerlinNoise.h"

#include <delaunator.hpp>

#include <cassert>
#include <cmath>
#include <array>
#include <fstream>

#include <unordered_map>


// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#pragma warning(disable : 4996)



#define PI 3.1415926f


Mesh GeometryHelper::CreatePlane(size_t xSlices, size_t ySlices, glm::vec3 up, float uScale, float vScale, std::function<float(float, float)> heightFunc, glm::vec3 tangent, glm::vec3 bitangent)
{
	// declare containers to hold all the data of the new plane we are creating
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	// if we are not provided with a tangent and bitangent we will calculate our own
	if (tangent == glm::vec3{0, 0, 0} || bitangent == glm::vec3{0, 0, 0})
	{
		tangent = { up.z, up.x, up.y };
		bitangent = glm::cross(tangent, up);
	}

	// work out the increment along each axis
	float xStep = 1.0f / (xSlices - 1);
	float yStep = 1.0f / (ySlices - 1);

	// begin in one corner
	float posX = -0.5f;
	float posY = 0.5f;

	// iterate for number of slices on each axis
	for (size_t y = 0; y < ySlices; y++)
	{
		for (size_t x = 0; x < xSlices; x++)
		{
			// calculate the position on the plane from the basis vectors and the height function
			glm::vec3 v{ tangent * posX + bitangent * posY + up * heightFunc(posX, posY) };

			// add the position and texture coordinate
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

	// recalculation of tangent and bitangent are required as the surface may no longer be flat due to the height function
	for (size_t i = 0; i < xSlices * ySlices; i++)
	{
		// tangent calculation
		// the tangent is the average change in position between adjacent points on one axis
		// bitangent is the same calculation on the perpendicular axis
		glm::vec3 tangent{0, 0, 0};
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
		// make sure no division by 0 occurs
		assert(factor != 0);
		tangent = { tangent.x / factor, tangent.y / factor, tangent.z / factor };


		// bi-tangent calculation
		glm::vec3 bitangent{0, 0, 0};
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
		// normal is the cross product of the tangent and bitangent
		glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));
		// add normal to vector
		normals.push_back(normal);
	}

	// now triangulate the plane
	// the plane can easily be split into quads, and then each quad split into 2 triangles
	unsigned int currentIndex = 0;
	for (size_t y = 0; y < ySlices - 1; y++)
	{
		for (size_t x = 0; x < xSlices - 1; x++)
		{
			// triangle 1
			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + xSlices);

			// triangle 2, to complete the quad
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + xSlices + 1);
			indices.push_back(currentIndex + xSlices);

			currentIndex++;
		}
		currentIndex++;
	}

	// return a new mesh created from the data we have calculated
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

	// a cube can be considered as 6 planes facing along all 6 axes combined together

	// the 6 directions each face looks in
	std::array<glm::vec3, 6> faceDirections =
	{
		glm::vec3{  1,  0,  0 },
		glm::vec3{ -1,  0,  0 },
		glm::vec3{  0,  1,  0 },
		glm::vec3{  0, -1,  0 },
		glm::vec3{  0,  0,  1 },
		glm::vec3{  0,  0, -1 }
	};
	for (const auto& dir : faceDirections)
	{
		// create a new plane that will form one face of the cube
		Mesh face = CreatePlane(resolution, resolution, dir, 1.0f, 1.0f, [](float x, float z)->float { return 0.5f; });
		// and combine it with the cube we have created so far to incrementally construct the cube
		CombineMeshes(unitCube, face);
	}
	return unitCube;
}

Mesh GeometryHelper::CreateDisc(float radius, size_t resolution)
{
	// change in angle between each vertex along the edge of the disc
	float deltaAngle = 2 * 3.1415926f / resolution;

	// create vectors to hold all the data of the disc
	// we know the size of all of them in advance just from the resolution
	std::vector<glm::vec3> positions(resolution + 1);
	std::vector<glm::vec3> normals(resolution + 1, { 0, 1, 0 });
	std::vector<glm::vec2> texCoords(resolution + 1);
	std::vector<unsigned int> triangles(3 * resolution);

	// one central vertex
	positions[0] = {0, 0, 0};
	texCoords[0] = { 0.5f, 0.5f };

	float angle = 0.0f;
	size_t triIndex = 0;
	for (size_t i = 0; i < resolution; i++)
	{
		// calculate position and tex coordinate
		positions[i + 1] = { radius * cosf(angle), 0, radius * sinf(angle) };
		texCoords[i + 1] = { 0.5f * (cosf(angle) + 1), 0.5f * (sinf(angle) + 1) };
		angle += deltaAngle;

		// construct a triangle connecting this vertex to its neighbour and the central vertex
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

Mesh GeometryHelper::CreateUnitSphere(size_t resolution)
{
	std::vector<glm::vec3> points(resolution);

	// we can use some golden ratio, fibonacci magic to create much better evenly distributed points on a sphere
	// which makes everyone happy

	// dlong = PI * (3 - sqrt(5))
	// stdlib sqrt is not constexpr, so computed manually and hard-coded instead to make it faster
	constexpr float dlong = 2.39996323f;
	float dy = 2.0f / resolution;
	float longitude = 0;
	float y = 1 - dy / 2.0f;
	for (size_t i = 0; i < resolution; i++)
	{
		float r = sqrtf(1 - y * y);
		points[i] = { cosf(longitude) * r, y, sinf(longitude) * r };
		y -= dy;
		longitude += dlong;
	}


	// sterographically project all the points on the sphere
	// so we can perform delaunay triangulation
	// format that the delaunator library expects: x0, y0, x1, y1...
	std::vector<double> projectedPoints(2 * resolution);
	for (size_t i = 0; i < resolution; i++)
	{
		glm::vec3 p = points[i];
		double X = p.x / (1.0 + p.y);
		double Z = p.z / (1.0 + p.y);

		projectedPoints[2 * i] = X;
		projectedPoints[(2 * i) + 1] = Z;
	}

	// triangulate using delaunator
	// a beautiful library from: https://github.com/delfrrr/delaunator-cpp
	delaunator::Delaunator d(projectedPoints);

	// the mesh expects a vector of unsigned ints, so we need to convert
	std::vector<unsigned int> triangles(d.triangles.size());
	for (size_t i = 0; i < triangles.size(); i++)
		triangles[i] = static_cast<unsigned int>(d.triangles[i]);

	// the mesh will contain a hole at the south pole which needs to be stitched up
	// this is a side effect of using a stereographic projection;
	// the south pole would be projected to infinity and therefore will not be included in the delauny triangulation

	// add a vertex at the south pole
	points.push_back({0, -1, 0});

	int n = points.size() - 1;

	// add 5 new triangles to stitch up the hole
	triangles.push_back(n);
	triangles.push_back(n - 5);
	triangles.push_back(n - 2);

	triangles.push_back(n);
	triangles.push_back(n - 2);
	triangles.push_back(n - 4);

	triangles.push_back(n);
	triangles.push_back(n - 4);
	triangles.push_back(n - 1);

	triangles.push_back(n);
	triangles.push_back(n - 1);
	triangles.push_back(n - 3);

	triangles.push_back(n);
	triangles.push_back(n - 3);
	triangles.push_back(n - 5);

	// define normals, simply the normalized position.
	// as this is a unit sphere the point should be normalized anyway
	// but nevermind, this doesnt get run every frame hopefully
	std::vector<glm::vec3> normals(points.size());
	for (size_t i = 0; i < resolution; i++)
		normals[i] = glm::normalize(points[i]);


	// calculate texture coordinates for every point
	std::vector<glm::vec2> texCoords(points.size());
	size_t i = 0;
	for (auto& p : points)
	{
		// calculate phi and theta, the parameters for the parametric equation of a sphere
		// ie longitude and latitude
		float phi = acosf(p.y);
		float theta = acosf(p.x / sinf(phi));

		// from there, we can easily map from longitude and latitude to normalized uv's for the texture mapping
		texCoords[i] = { theta / PI, phi / PI };
		i++;
	}


	// return mesh
	return Mesh{
		points,
		normals,
		texCoords,
		triangles
	};

}

Mesh GeometryHelper::CreateCylinder(float height, float radius, size_t resolution)
{
	// create the top cap of the cylinder
	Mesh topCap = CreateDisc(radius, resolution);
	Mesh bottomCap = CreateDisc(radius, resolution);
	// there are resolution + 1 points in each mesh
	for (size_t i = 0; i < resolution + 1; i++)
	{
		topCap.Vertices[i].Position.y += height * 0.5f;
		bottomCap.Vertices[i].Position.y -= height * 0.5f;
		bottomCap.Vertices[i].Normal.y *= -1;
	}
	// combine the top cap and bottom cap into one mesh
	CombineMeshes(topCap, bottomCap);

	Mesh side;
	// begin with the same vertices from the top and bottom cap mesh,
	// but we adjust their texture coordinates, normals, and create new indices to join them together differently
	side.Vertices = topCap.Vertices;

	for (size_t i = 1; i < resolution + 1; i++)
	{
		// create 2 triangles connecting the top and bottom cap
		// lots of if...else... to deal with the triangles wrapping round the cylinder
		side.Indices.push_back(i);
		if (i + resolution + 2 >= side.Vertices.size())
			side.Indices.push_back(resolution + 2);
		else
			side.Indices.push_back(i + resolution + 2);
		side.Indices.push_back(i + resolution + 1);

		side.Indices.push_back(i);
		if (i + 1 >= resolution + 1)
			side.Indices.push_back(1);
		else
			side.Indices.push_back(i + 1);
		if (i + resolution + 2 >= side.Vertices.size())
			side.Indices.push_back(resolution + 2);
		else
			side.Indices.push_back(i + resolution + 2);
	}

	for (auto& vertex : side.Vertices)
	{
		// finally calculate the normals and texture coordinates for the side of the cylinder
		glm::vec3 n = vertex.Position / radius;
		vertex.Normal = glm::vec3(n.x, 0.0f, n.y);

		// convert to parametric equation of a circle to calculate texture coordinates
		float theta = acosf(vertex.Position.x / radius);
		vertex.TexCoord = glm::vec2(theta / (2 * PI), vertex.Position.y > 0 ? 0 : 1);
	}

	// finally combine our meshes and return them as a single mesh
	CombineMeshes(topCap, side);
	return topCap;
}

Mesh GeometryHelper::CreateIcosahedron()
{
	// from https://www.danielsieger.com/blog/2021/01/03/generating-platonic-solids.html

	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	float a = 1.0f;
	float b = 1.0f / phi;

	// add vertices
	std::vector<glm::vec3> positions = {
		{ 0, b, -a },
		{ b, a, 0 },
		{ -b, a, 0 },
		{ 0, b, a },
		{ 0, -b, a },
		{ -a, 0, b },
		{ 0, -b, -a },
		{ a, 0, -b },
		{ a, 0, b },
		{ -a, 0, -b },
		{ b, -a, 0 },
		{ -b, -a, 0 }
	};

	for (auto& pos : positions) pos = glm::normalize(pos);

	// add triangles
	std::vector<unsigned int> indices = {
		2, 1, 0,
		1, 2, 3,
		5, 4, 3,
		4, 8, 3,
		7, 6, 0,
		6, 9, 0,
		11, 10, 4,
		10, 11, 6,
		9, 5, 2,
		5, 9, 11,
		8, 7, 1,
		7, 8, 10,
		2, 5, 3,
		8, 1, 3,
		9, 2, 0,
		1, 7, 0,
		11, 9, 6,
		7, 10, 6,
		5, 11, 4,
		10, 8, 4
	};

	return Mesh(positions, positions, indices);
}



Mesh GeometryHelper::LoadObj(const std::string& filename)
{
	// heavily modified from the code we were provided in the labs, to work seamlessly with my Mesh class

	// these arrays may not be the same size after loading from the file!
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	// map from an obj format index (v/n/t) to a constructed Vertex object inside the vertices vector
	std::unordered_map<std::string, unsigned int> indexMap;

	// constructed model data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// open file, make sure it opened successfully
	FILE* file = fopen(filename.c_str(), "r");
	if (file == NULL)
	{
		assert(false && "Failed to open file");
		std::abort();
	}
	// loop until we hit the end of the file
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
				glm::vec3 pos;
				int matches = fscanf(file, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
				assert((matches == 3) && "Failed to parse model");
				positions.push_back(pos);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				glm::vec2 uv;
				int matches = fscanf(file, "%f %f\n", &uv.x, &uv.y);
				assert((matches == 2) && "Failed to parse model");
				uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				glm::vec3 normal;
				int matches = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				assert((matches == 3) && "Failed to parse model");
				normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];

				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2],
																					&face[3], &face[4],  &face[5],
																					&face[6], &face[7],  &face[8]);
				if (matches != 9)
				{
					assert(false && "Failed to parse model");
					std::abort();
				}

				// loop through the 3 vertices
				for (size_t i = 0; i < 3; i++)
				{
					// construct a string to identify the pos, normal and uv that we require
					std::string id = std::to_string(face[ 3 * i     ]) + "/" +
									 std::to_string(face[(3 * i) + 1]) + "/" +
									 std::to_string(face[(3 * i) + 2]);
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
						newVertex.Position = positions[face[3 * i] - 1];
						if (uvs.size() > 0) newVertex.TexCoord = uvs[face[(3 * i) + 1] - 1];
						if (normals.size() > 0) newVertex.Normal = normals[face[(3 * i) + 2] - 1];

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

	// always remember to close the file
	fclose(file);

	// return a mesh constructed using the file data
	return Mesh(vertices, indices);
}

void GeometryHelper::CombineMeshes(Mesh& a, Mesh& b)
{
	// offset the indices in b by how many vertices are already in a
	size_t aVertexCount = a.Vertices.size();
	for (auto& index : b.Indices) index += aVertexCount;

	// then just insert all data from b into a
	a.Vertices.insert(a.Vertices.end(), b.Vertices.begin(), b.Vertices.end());

	a.Indices.insert(a.Indices.end(), b.Indices.begin(), b.Indices.end());
}


#pragma region HeightFuncs


// from https://en.wikipedia.org/wiki/Smoothstep
float clamp(float x, float lowerlimit, float upperlimit) {
	if (x < lowerlimit)
		x = lowerlimit;
	if (x > upperlimit)
		x = upperlimit;
	return x;
}


float smootherstep(float edge0, float edge1, float x) {
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	// Evaluate polynomial
	return x * x * x * (x * (x * 6 - 15) + 10);
}



float GeometryHelper::HeightFuncs::PerlinNoiseTerrain(float x, float z)
{
	// I just hard-coded values for the one time this was used in my coursework
	// ideally I would be able to configure all these parameters to make lots of cool terrain
	static PerlinNoise noise;
	
	// create a flat centre area in the terrain, for the portal and other stuff
	float smoothingValue = smootherstep(0.005f, 0.05f, fabsf(x * x + z * z));

	// create some cool bumpy terrain
	float noiseValue = 0.0f;
	float s = 3.2f;
	float f = 4;
	for (int i = 0; i < 4; i++)
	{
		noiseValue += s * (float)noise.noise(f * (double)x, 0, f * (double)z);
		f *= 2.3f;
		s *= 0.5f;
	}

	return (1.2f * noiseValue - 1.0f) * smoothingValue;
}

#pragma endregion