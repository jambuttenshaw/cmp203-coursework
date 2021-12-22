#include "Portal.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Application.h"

#include "Objects/PortalScene.h"

#include "Rendering/GeometryHelper.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/Camera.h"

#include <cassert>
#include <iostream>
#include <sstream>


static float gLastSideOfPortal = 0;
bool Portal::sPortalRenderInProgress = false;

Portal::Portal(PortalScene* sceneToRender)
{
	mScreenModel = GeometryHelper::LoadObj("models/portalScreen.obj");
	mFrameModel = GeometryHelper::LoadObj("models/portal.obj");
	mSceneToRender = sceneToRender;
}

void Portal::TestForTravelling(Input* in, Camera* traveller)
{
	glm::vec3 a = traveller->getPosition() - mTransform.GetTranslation();
	glm::vec3 b = mTransform.LocalToWorld() * glm::vec4(0, 0, 1, 0);

	float sideOfPortal = glm::sign(glm::dot(a, b));

	if ((gLastSideOfPortal != sideOfPortal) && (gLastSideOfPortal != 0))
	{
		// check to make sure the traveller actually passed through the inside of the portal
		glm::vec3 localPos = mTransform.WorldToLocal() * glm::vec4(traveller->getPosition(), 1);
		if (glm::abs(localPos.x) < 0.5f && localPos.y > 0 && localPos.y < 2)
		{
			// we have travelled!
			// switch scenes and move the traveller
			if (mLinkedPortal != nullptr)
			{
				Scene* newScene = mLinkedPortal->mSceneToRender;
				Application::SetActiveScene(newScene);

				Camera* cam = newScene->GetActiveCamera();

				cam->setPosition(traveller->getPosition() + 0.1f * traveller->getMoveDirection() - mTransform.GetTranslation() + mLinkedPortal->mTransform.GetTranslation());
				cam->setPitch(traveller->getPitch());
				cam->setYaw(traveller->getYaw());

				mLinkedPortal->SetSideOfPortal(sideOfPortal);
			}
		}
	}
	gLastSideOfPortal = sideOfPortal;
}

void Portal::Render()
{
	assert(mSceneToRender->PortalPassActive());

	// dont draw this portal if were already rendering one to the screen!
	if (sPortalRenderInProgress) return;


	// mark portal rendering as begun
	sPortalRenderInProgress = true;

	// preserve the outside state
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// draw portal screen into stencil buffer

	// disable writing to frame buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	// enable & setup stencil test
	glEnable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 0x10, 0XF0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// draw our stencil
	{
		Transformation t(mTransform);
		{
			// an additional transformation to avoid z fighting
			const float adjustment = 0.0001f;
			Transformation t2({ 0, adjustment * 0.5f, 0 }, { 0, 0, 0 }, { 1, 1 - adjustment, 1 });
			RenderHelper::drawMesh(mScreenModel);
		}
	}
	glDisable(GL_CULL_FACE);


	// re-enable writing to frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// we do not want to change the stencil buffer from here
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	// render everything on one side of the portal
	glStencilFunc(GL_EQUAL, 0x10, 0XF0);

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
				s->render(mSceneToRender->GetActiveCamera()->getPosition() - mTransform.GetTranslation() + mLinkedPortal->mTransform.GetTranslation());
			}

			// render the scene that the linked portal looks into


			// align the near clipping plane with the portal screen
			// work out the equation of the plane of the screen
			const float nearClipOffset = 0.1f;

			Camera* cam = mSceneToRender->GetActiveCamera();
			
			glm::vec3 d{ glm::normalize(mTransform.GetTranslation() - cam->getPosition()) };
			glm::vec3 f{ cam->getForward() };
			float angle = glm::dot(d, f);

			Scene::setNearPlane(glm::max(0.05f, angle * distanceToPortal));
			
			Skybox::DisableSkyboxRendering();
			mLinkedPortal->mSceneToRender->RenderSceneLights();
			mLinkedPortal->mSceneToRender->OnRenderObjects();
			Skybox::EnableSkyboxRendering();
			
			Scene::setNearPlane(0.05f);
		}
		glPopAttrib();


		// now we want to fill in the depth information of the portal, since everything behind it has already been drawn
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthFunc(GL_ALWAYS);
		{
			Transformation t(mTransform);
			{
				// an additional transformation to avoid z fighting
				const float adjustment = 0.0001f;
				Transformation t2({ 0, adjustment * 0.5f, 0 }, { 0, 0, 0 }, { 1, 1 - adjustment, 1 });
				RenderHelper::drawMesh(mScreenModel);
			}
		}
		glDepthFunc(GL_LEQUAL);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

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
			Transformation t(mTransform);
			RenderHelper::drawMesh(mScreenModel);
		}

		glPopAttrib();
	}
	
	glDisable(GL_STENCIL_TEST);


	Material::Default.apply();
	{
		Transformation t(mTransform);
		{
			const float scaleAdjustment = 0.001f;
			Transformation adjustement({ 0, 0, 0 }, { 0, 0, 0 }, glm::vec3{ 1 - scaleAdjustment });
			RenderHelper::drawMesh(mFrameModel);
		}
	}

	glPopAttrib();


	// portal render is finished
	sPortalRenderInProgress = false;
}
