#pragma once

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include <glm/glm.hpp>

class Shadow
{
public:
	static void generateShadowMatrix(float* shadowMatrix, float light_pos[4], GLfloat floor[12]);
	static void generateShadowMatrix(float* shadowMatrix, const glm::vec3& lightPos, const glm::vec3& floorNormal, const glm::vec3& pointOnFloor);
	static void extendVertex(float newVert[3], float lightPosit[4], float x, float y, float z, float ext);
	static std::vector<float> buildShadowVolume(float lightPosit[4], std::vector<float> verts);
};