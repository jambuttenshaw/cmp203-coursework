#include "Model.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

void Model::transformAndRenderHeirarchy()
{
	glPushMatrix();

	glTranslatef(mTranslation.x, mTranslation.y, mTranslation.z);
	glRotatef(mRotation.x, 1, 0, 0);
	glRotatef(mRotation.y, 0, 1, 0);
	glRotatef(mRotation.z, 0, 0, 1);
	glScalef(mScale.x, mScale.y, mScale.z);

	render();

	for (auto child : mChildren) child->transformAndRenderHeirarchy();

	glPopMatrix();
}

void Model::render()
{
	glBegin(GL_TRIANGLES);

	for (auto i : indices)
	{
		const Vector3& vert = vertices[i];
		glVertex3f(vert.x, vert.y, vert.z);
	}

	glEnd();
}

void Model::setParent(Model* parent)
{
	mParent->removeChild(this);
	mParent = parent;

	if (parent != nullptr)
		mParent->addChild(this);
}

void Model::addChild(Model* child)
{
	if (child->getParent() != nullptr)
		child->getParent()->removeChild(child);

	child->setParent(this);
	mChildren.push_back(child);

}

void Model::removeChild(Model* child)
{
	for (size_t i = 0; i < mChildren.size(); i++)
	{
		if (mChildren[i] == child)
		{
			mChildren.erase(std::next(mChildren.begin(), i));
			return;
		}
	}
}