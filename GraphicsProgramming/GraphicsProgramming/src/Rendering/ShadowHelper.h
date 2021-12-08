#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <set>

#include "Mesh.h"
#include "Objects/GameObject.h"


class ShadowHelper
{
public:

	// calculate shadow matrix
	static void CalculateShadowMatrix(glm::mat4& out, const glm::vec3& lightPos, const glm::vec3& planeNormal, const glm::vec3& pointOnPlane);

	// build shadow volume
	static Mesh BuildShadowVolume(const Mesh& mesh, const glm::mat4& transform, const glm::vec3& lightPos);
	static Mesh BuildShadowVolume(const GameObject& gameObject, const glm::vec3& lightPos);

private:
	// extend vertex helper function
	static glm::vec3 ExtendVertex(const glm::vec3& pos, const glm::vec3& lightPos, float extension);

	static std::set<std::pair<size_t, size_t>> CalculateSilhouette(const Mesh& mesh, const glm::mat4& transform, const glm::vec3& lightPos);

};