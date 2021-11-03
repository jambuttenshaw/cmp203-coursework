#pragma once

#include <string>
#include <unordered_map>

#include "Texture.h"
#include "Core/Vector2.h"
#include "Core/Vector3.h"
#include "Mesh.h"


class Skybox
{
public:

	enum class Face
	{
		Right,
		Left,
		Top,
		Bottom,
		Front,
		Back
	};

public:
	Skybox(const std::string& filepath);
	~Skybox();


	void render(const Vector3& position);

private:

	struct FaceData
	{
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
		Vector2 uvOffset;
		Mesh planeMesh;
	};

	Texture mTexture;
	std::unordered_map<Face, FaceData> mFaceData;

};

