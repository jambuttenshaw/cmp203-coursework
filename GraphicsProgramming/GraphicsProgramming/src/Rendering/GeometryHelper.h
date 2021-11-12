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
							Vector3 up = Vector3::up,
							float uScale = 1.0f, float vScale = 1.0f,
							std::function<float(float, float)> heightFunc = HeightFuncs::Flat,
							Vector3 tangent = Vector3::zero, Vector3 bitangent = Vector3::zero);

	static Mesh CreateUnitCube(size_t resolution);
	static Mesh CreateUnitSphere(size_t resolution);
	static Mesh CreateDisc(float radius, size_t resolution);
	static Mesh CreateCylinder(float height, float radius, size_t resolution);

	static void CombineMeshes(Mesh& a, Mesh& b);
};
