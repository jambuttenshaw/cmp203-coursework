#pragma once

#include "Rendering/Transform.h"
#include "Rendering/Texture.h"
#include "Rendering/Mesh.h"

/// <summary>
///		A simple helper class that combines a mesh and a transform,
///		to allow for less member variables in each scene class
/// </summary>
class GameObject
{
public:
	GameObject();
	~GameObject();

	inline Mesh& GetMesh() const { return *mMesh; }
	inline Transform& GetTransform() const { return *mTransform; }
	
protected:
	Mesh* mMesh = nullptr;
	Transform* mTransform = nullptr;
};
