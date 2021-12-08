#include "GameObject.h"

GameObject::GameObject()
{
	mMesh = new Mesh;
	mTransform = new Transform;
}

GameObject::~GameObject()
{
	delete mMesh;
	delete mTransform;
}
