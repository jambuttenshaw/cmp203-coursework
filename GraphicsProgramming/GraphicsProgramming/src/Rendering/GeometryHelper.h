#pragma once

#include <functional>

#include "Mesh.h"


class GeometryHelper
{
public:
	class HeightFuncs
	{
	public:
		// default height funcs
		static float Flat(float x, float y) { return 0; }
		static float PerlinNoiseTerrain(float x, float z);
	};

public:
	GeometryHelper() = delete;

	static Mesh CreatePlane(size_t xSlices, size_t ySlices,
							glm::vec3 up = {0, 1, 0},
							float uScale = 1.0f, float vScale = 1.0f,
							std::function<float(float, float)> heightFunc = HeightFuncs::Flat,
							glm::vec3 tangent = {0, 0, 0}, glm::vec3 bitangent = {0, 0, 0});

	static Mesh CreateUnitCube(size_t resolution);
	static Mesh CreateUnitSphere(size_t resolution);
	static Mesh CreateDisc(float radius, size_t resolution);
	static Mesh CreateCylinder(float height, float radius, size_t resolution);

	static Mesh LoadObj(const std::string& filename);

	static void CombineMeshes(Mesh& a, Mesh& b);
};
