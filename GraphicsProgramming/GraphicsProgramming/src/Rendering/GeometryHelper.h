#pragma once

#include <functional>

#include "Mesh.h"


class GeometryHelper
{
public:
	class HeightFuncs
	{
	public:
		HeightFuncs() = delete;

		// default height funcs that can be used to shape planes
		// height
		static float Flat(float x, float y) { return 0; }
		static float PerlinNoiseTerrain(float x, float z);
	};

public:
	// static class: do not instantiate
	GeometryHelper() = delete;

	// create a plane
	// this allows for a lot of customization
	static Mesh CreatePlane(size_t xSlices, size_t ySlices, // number of vertices along each base direction of the plane
							glm::vec3 up = {0, 1, 0}, // the 'up' direction of the plane, for a flat plane this would be the normal of every point
							float uScale = 1.0f, float vScale = 1.0f, // the range that the uv's go along the plane,
																	  // can be used to control how many times a texture will tile across the plane
							std::function<float(float, float)> heightFunc = HeightFuncs::Flat, // the height func determines the 3rd dimension of each point on the plane
																							   // it takes the position on the plane in the other 2 dimensions as input
							glm::vec3 tangent = {0, 0, 0}, glm::vec3 bitangent = {0, 0, 0});   // the tangent and bitangent control the direction that the basis vectors
																							   // point in when generating points on the plane before applying the height
																							   // func. the final tangent and bitangents for each point are recalculated
																							   // after the height function has been applied

	// create a unit cube with 'resolution' number of vertices along each edge
	static Mesh CreateUnitCube(size_t resolution);
	// create a unit sphere out of 'resolution' number of vertices
	static Mesh CreateUnitSphere(size_t resolution);
	// create a disc of specified radius, with 'resolution' number of vertices along its edge
	static Mesh CreateDisc(float radius, size_t resolution);
	// create a cylinder with dimensions specified, with 'resolution' vertices along the edge of each disc capping the cylinder
	static Mesh CreateCylinder(float height, float radius, size_t resolution);
	// create an icosahedron
	static Mesh CreateIcosahedron();

	// load a mesh from an obj file found at 'filename'
	static Mesh LoadObj(const std::string& filename);

	// combine two meshes into 'a'
	static void CombineMeshes(Mesh& a, Mesh& b);
};
