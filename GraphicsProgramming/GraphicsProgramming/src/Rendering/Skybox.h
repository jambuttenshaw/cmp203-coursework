#pragma once

#include <string>
#include <unordered_map>

#include "Texture.h"
#include "Core/Vector2.h"
#include "Core/Vector3.h"
#include "Mesh.h"


class Skybox : public Mesh
{
public:
	Skybox(const std::string& filepath);
	~Skybox();

	void render(const Vector3& position);

private:
	void Rebuild();
};

