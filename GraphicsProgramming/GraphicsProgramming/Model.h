#pragma once

#include <vector>

#include "Vector3.h"


class Model
{
public:

	void transformAndRenderHeirarchy();

	void setParent(Model* parent);
	inline Model* getParent() { return mParent; }

	void addChild(Model* child);
	void removeChild(Model* child);

	inline void setTranslation(const Vector3& t) { mTranslation = t; }
	inline void translate(const Vector3& t) { mTranslation += t; }
	inline void setRotation(const Vector3& r) { mRotation = r; }
	inline void rotate(const Vector3& r) { mRotation += r; }
	inline void setScale(const Vector3& s) { mScale = s; }
	inline void scale(const Vector3& s) { mScale += s; }
	
	inline void setVertices(std::vector<Vector3> verts) { vertices = verts; }
	inline void setIndices(std::vector<int> inds) { indices = inds; }

private:

	void render();

private:
	// transformation
	Vector3 mTranslation = { 0, 0, 0 };
	Vector3 mRotation = { 0, 0, 0 };
	Vector3 mScale = { 1, 1, 1 };

	// scene heirarchy
	std::vector<Model*> mChildren;
	Model* mParent;

	// geometry
	std::vector<Vector3> vertices;
	std::vector<int> indices;
};

