#pragma once

#include "Rendering/Transform.h"
#include "Rendering/Texture.h"
#include "Rendering/Mesh.h"


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
