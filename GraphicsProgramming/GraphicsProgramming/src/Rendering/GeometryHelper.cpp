#include "GeometryHelper.h"

#include "PerlinNoise.h"

#include <delaunator.hpp>

#include <cassert>
#include <cmath>
#include <array>


#define PI 3.1415926f


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

Mesh GeometryHelper::CreateUnitSphere(size_t resolution)
{
	std::vector<Vector3> points(resolution);

	// dlong = PI * (3 - sqrt(5))
	// sqrt is not constexpr, so computed manually and hard-coded instead
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
	// format is: x0, y0, x1, y1...
	std::vector<double> projectedPoints(2 * resolution);
	for (size_t i = 0; i < resolution; i++)
	{
		Vector3 p = points[i];
		double X = p.x / (1.0 + p.y);
		double Z = p.z / (1.0 + p.y);

		projectedPoints[2 * i] = X;
		projectedPoints[(2 * i) + 1] = Z;
	}

	// triangulate using delaunator
	delaunator::Delaunator d(projectedPoints);

	// the mesh expects a vector of unsigned ints, so we need to convert
	std::vector<unsigned int> triangles(d.triangles.size());
	for (size_t i = 0; i < triangles.size(); i++)
		triangles[i] = static_cast<unsigned int>(d.triangles[i]);

	// the mesh will contain a hole at the south pole which needs to be stitched up
	// this is a side effect of using a stereographic projection; the south pole would be projected to infinity

	// add a vertex at the south pole
	points.push_back(Vector3::down);

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

	// define normals
	std::vector<Vector3> normals(points.size());
	for (size_t i = 0; i < resolution; i++)
		normals[i] = points[i].normalised();


	// calculate texture coordinates for every point
	std::vector<Vector2> texCoords(points.size());
	size_t i = 0;
	for (auto& p : points)
	{
		float phi = acosf(p.y);
		float theta = acosf(p.x / sinf(phi));

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

Mesh GeometryHelper::CreateDisc(float radius, size_t resolution)
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
		positions[i + 1] = { radius * cosf(angle), 0, radius * sinf(angle) };
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

	CombineMeshes(topCap, bottomCap);

	for (size_t i = 1; i < resolution + 1; i++)
	{
		// create 2 triangles connecting the top and bottom cap
		topCap.Indices.push_back(i);
		if (i + resolution + 2 >= topCap.Vertices.size())
			topCap.Indices.push_back(resolution + 2);
		else
			topCap.Indices.push_back(i + resolution + 2);
		topCap.Indices.push_back(i + resolution + 1);

		topCap.Indices.push_back(i);
		if (i + 1 >= resolution + 1)
			topCap.Indices.push_back(1);
		else
			topCap.Indices.push_back(i + 1);
		if (i + resolution + 2 >= topCap.Vertices.size())
			topCap.Indices.push_back(resolution + 2);
		else
			topCap.Indices.push_back(i + resolution + 2);
	}
	return topCap;
}

void GeometryHelper::CombineMeshes(Mesh& a, Mesh& b)
{
	// offset the indices in b by how many vertices are already in a
	size_t aVertexCount = a.Vertices.size();
	for (auto& index : b.Indices) index += aVertexCount;

	a.Vertices.insert(a.Vertices.end(), b.Vertices.begin(), b.Vertices.end());

	a.Indices.insert(a.Indices.end(), b.Indices.begin(), b.Indices.end());
}

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
