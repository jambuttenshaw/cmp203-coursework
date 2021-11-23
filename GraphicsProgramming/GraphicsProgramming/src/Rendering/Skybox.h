#pragma once

#include <string>
#include <unordered_map>

#include "Texture.h"
#include <glm/glm.hpp>
#include "Mesh.h"


class Skybox : public Mesh
{
public:
	Skybox(const std::string& filepath);
	~Skybox();

	void render(const glm::vec3& position);

private:
	void Rebuild();
};

