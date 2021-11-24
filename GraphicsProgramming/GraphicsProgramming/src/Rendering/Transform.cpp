#include "Transform.h"

glm::mat4 Transform::LocalToWorld() const
{
	glm::mat4 m = glm::translate(mTranslation);
	m *= glm::rotate(mRotation.x, glm::vec3{ 1, 0, 0 });
	m *= glm::rotate(mRotation.y, glm::vec3{ 0, 1, 0 });
	m *= glm::rotate(mRotation.z, glm::vec3{ 0, 0, 1 });
	m *= glm::scale(mScale);

	return m;
}

glm::mat4 Transform::WorldToLocal() const
{
	return glm::inverse(LocalToWorld());
}

void Transform::Identity()
{
	mTranslation = glm::vec3(0, 0, 0);
	mRotation = glm::vec3(0, 0, 0);
	mScale = glm::vec3(0, 0, 0);
}

