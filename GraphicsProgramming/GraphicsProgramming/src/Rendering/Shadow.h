#pragma once

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include "Core/Vector3.h"

class Shadow
{
public:
	static void generateShadowMatrix(float* shadowMatrix, float light_pos[4], GLfloat floor[12]);
	static void generateShadowMatrix(float* shadowMatrix, const Vector3& lightPos, const Vector3& floorNormal, const Vector3& pointOnFloor);
	static void extendVertex(float newVert[3], float lightPosit[4], float x, float y, float z, float ext);
	static std::vector<float> buildShadowVolume(float lightPosit[4], std::vector<float> verts);
};