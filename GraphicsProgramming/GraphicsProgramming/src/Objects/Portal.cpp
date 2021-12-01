#include "Portal.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/Application.h"

#include "Objects/PortalScene.h"

#include "Rendering/GeometryHelper.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/Camera.h"

#include <cassert>
#include <windows.h>
#include <sstream>


bool Portal::sPortalRenderInProgress = false;

Portal::Portal(PortalScene* sceneToRender)
{
	mFrameModel = GeometryHelper::LoadObj("models/portal.obj");
	mScreenModel = GeometryHelper::LoadObj("models/portalScreen.obj");

	mSceneToRender = sceneToRender;
}

void Portal::TestForTravelling(Input* in, Camera* traveller)
{

	//std::stringstream s;
	//s << "This side of portal: " << mLastSideOfPortal << "\n"

	glm::vec3 a = traveller->getPosition() - mTransform.GetTranslation();
	glm::vec3 b = mTransform.LocalToWorld() * glm::vec4(0, 0, 1, 0);

	float sideOfPortal = glm::sign(glm::dot(a, b));

	if ((mLastSideOfPortal != sideOfPortal) && (mLastSideOfPortal != 0))
	{
		// check to make sure the traveller actually passed through the inside of the portal
		glm::vec3 localPos = mTransform.WorldToLocal() * glm::vec4(traveller->getPosition(), 1);
		if (glm::abs(localPos.x) < 0.5f)
		{
			// we have travelled!
			// switch scenes and move the traveller
			if (mLinkedPortal != nullptr)
			{
				Scene* newScene = mLinkedPortal->mSceneToRender;
				Application::SetActiveScene(newScene);

				Camera* cam = newScene->GetActiveCamera();
				cam->setPosition(traveller->getPosition());
				cam->setPitch(traveller->getPitch());
				cam->setYaw(traveller->getYaw());

				mLinkedPortal->SetSideOfPortal(sideOfPortal);
			}
		}
	}
	mLastSideOfPortal = sideOfPortal;
}

void Portal::Render()
{
	// dont draw this portal if were already rendering one to the screen!
	if (sPortalRenderInProgress)
	{
		// just draw the frame
		{
			Transformation t(mTransform.GetTranslation(), mTransform.GetRotation(), mTransform.GetScale());
			RenderHelper::drawMesh(mFrameModel);
		}
		return;
	}
	// mark portal rendering as begun
	sPortalRenderInProgress = true;

	// draw portal screen into stencil buffer

	// disable writing to frame buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	// enable & setup stencil test
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// draw our stencil
	{
		Transformation t(mTransform.GetTranslation(), mTransform.GetRotation(), mTransform.GetScale());
		RenderHelper::drawMesh(mScreenModel);
	}
	

	// re-enable writing to frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// we do not want to change the stencil buffer from here
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


	// render everything on one side of the portal
	glStencilFunc(GL_EQUAL, 1, 0xFF);

	// this is where we want to render another scene
	// check to make sure theres actually a portal linked to this one
	if (mLinkedPortal)
	{
		// transform the point of view of the camera to be looking into the linked scene
		// from its current pov in this scene
		
		// we also need to move the near clipping plane to just in front of the portal
		// so that no objects get rendered between the camera and the portal

		glm::mat4 m = mTransform.LocalToWorld() * mLinkedPortal->GetTransform().WorldToLocal();
		glm::vec3 pos = m[3];

		// get rotations
		glm::quat q(m);
		glm::vec3 euler = glm::eulerAngles(q);

		// get distance from pos to portal
		glm::vec3& a = mSceneToRender->GetActiveCamera()->getPosition() - GetTransform().GetTranslation();
		float distanceToPortal = glm::length(glm::vec2(a.x, a.z));

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		{
			Transformation t(pos, euler, { 1, 1, 1 });

			// we need to render the skybox at a different position than the scene would render it itself
			const Skybox* s = mLinkedPortal->mSceneToRender->GetSkybox();
			if (s != nullptr)
			{
				s->render(mSceneToRender->GetActiveCamera()->getPosition());
			}

			// render the scene that the linked portal looks into
			Skybox::DisableSkyboxRendering();
			mLinkedPortal->mSceneToRender->OnRenderObjects();
			Skybox::EnableSkyboxRendering();
		}
		glPopAttrib();
	}
	else
	{
		// there is no linked portal: just fill the screen with *magenta*
		// to save the material that is currently applied
		glPushAttrib(GL_ENABLE_BIT);

		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glColor3fv(Color::Magenta.ptr());
		{
			Transformation t(mTransform.GetTranslation(), mTransform.GetRotation(), mTransform.GetScale());
			RenderHelper::drawMesh(mScreenModel);
		}

		glPopAttrib();
	}

	// now we want to fill in the depth information of the portal, since everything behind it has already been drawn
	glDisable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	{
		Transformation t(mTransform.GetTranslation(), mTransform.GetRotation(), mTransform.GetScale());
		RenderHelper::drawMesh(mScreenModel);
	}
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// finally render the frame of the portal
	{
		Transformation t(mTransform.GetTranslation(), mTransform.GetRotation(), mTransform.GetScale());
		RenderHelper::drawMesh(mFrameModel);
	}

	// portal render is finished
	sPortalRenderInProgress = false;
}
