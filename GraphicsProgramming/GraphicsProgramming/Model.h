#pragma once

#include <vector>
#include <functional>

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

	inline void setRenderFunc(std::function<void(void)> renderFunc) { mRenderFunc = renderFunc; }

	inline void setColour(const Vector3& color) { mColor = color; }

private:

	void render();

private:
	// transformation
	Vector3 mTranslation = { 0, 0, 0 };
	Vector3 mRotation = { 0, 0, 0 };
	Vector3 mScale = { 1, 1, 1 };

	// scene heirarchy
	std::vector<Model*> mChildren;
	Model* mParent = nullptr;

	// geometry
	std::vector<Vector3> vertices;
	std::vector<int> indices;

	// rendering
	std::function<void(void)> mRenderFunc;
	Vector3 mColor = { 1, 1, 1 };
};

