#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Transformation.h"


class Transform
{
public:
	Transform() = default;


	glm::mat4 LocalToWorld() const;
	glm::mat4 WorldToLocal() const;
	void Identity();

	inline void SetTranslation(const glm::vec3& t) { mTranslation = t; }
	inline void Translate(const glm::vec3& t) { mTranslation += t; }
	inline const glm::vec3& GetTranslation() const { return mTranslation; }

	inline void SetRotation(const glm::vec3& r) { mRotation = r; }
	inline void Rotate(const glm::vec3& r) { mRotation += r; }
	inline const glm::vec3& GetRotation() const { return mRotation; }

	inline void SetScale(const glm::vec3& s) { mScale = s; }
	inline void Scale(const glm::vec3& s) { mScale += s; }
	inline const glm::vec3& GetScale() const { return mScale; }

private:
	glm::vec3 mTranslation{ 0.0f };
	glm::vec3 mRotation{ 0.0f };
	glm::vec3 mScale{ 1.0f };
};
