#include "Billboard.h"

#include "Rendering/Transformation.h"

#include "freeglut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


Billboard::Billboard(Texture* tex)
	: mTex(tex)
{
	gameObject.GetTransform().SetRotation({ 0, 0, 0 });

	renderObject = [this]() { this->Render(); };
}

void Billboard::Face(const glm::vec3& pos)
{
	// calculate the yaw to face the camera
	glm::vec3 toCam = pos - gameObject.GetTransform().GetTranslation();

	// only consider the XZ components for yaw
	glm::vec3 dir = glm::normalize(glm::vec3{ toCam.x, 0, toCam.z });
	float cosAngle = glm::dot(dir, glm::vec3{ 1, 0, 0 });
	float whichSide = glm::dot(dir, glm::vec3{ 0, 0, 1 });

	float yaw = glm::degrees(glm::acos(cosAngle));
	yaw = whichSide > 0 ? yaw : 360 - yaw;

	// yaw requires being offset by 90 degrees
	yaw += 90;

	// using cylindrical billboarding
	gameObject.GetTransform().SetRotation({ 90, 0, yaw });
}

void Billboard::Render()
{
	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColor3fv(Color::White.ptr());

	Transformation t(gameObject.GetTransform());
	RenderHelper::drawQuad(mTex);

	glPopAttrib();
}
