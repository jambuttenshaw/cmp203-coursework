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


float Portal::sLastSideOfPortal = 0.0f;
bool Portal::sPortalUpdateInProgress = false;
bool Portal::sPortalRenderInProgress = false;

Portal::Portal(PortalScene* sceneToRender)
{
	// load the models used by the portal
	mScreenModel = GeometryHelper::LoadObj("models/portalScreen.obj");
	mFrameModel = GeometryHelper::LoadObj("models/portal.obj");
	mSceneToRender = sceneToRender;
}

void Portal::Update(float dt, Input* in, Camera* traveller)
{
	// only update the portals if this wasnt called by another portal updating
	if (sPortalUpdateInProgress) return;
	sPortalUpdateInProgress = true;

	// update the scene that is visible through the portal
	mLinkedPortal->mSceneToRender->OnUpdate(dt);

	// work out which side of the portal we are on
	glm::vec3 a = traveller->getPosition() - mTransform.GetTranslation();
	glm::vec3 b = mTransform.LocalToWorld() * glm::vec4(0, 0, 1, 0);

	float sideOfPortal = glm::sign(glm::dot(a, b));

	// check if weve switched sides of the portal
	if ((sLastSideOfPortal != sideOfPortal) && (sLastSideOfPortal != 0))
	{
		// check to make sure the traveller actually passed through the inside of the portal
		glm::vec3 localPos = mTransform.WorldToLocal() * glm::vec4(traveller->getPosition(), 1);
		if (glm::abs(localPos.x) < 0.5f && localPos.y > 0 && localPos.y < 2)
		{
			// we have travelled!
			// switch scenes and move the traveller
			if (mLinkedPortal != nullptr)
			{
				// get the new scene and set it as active
				Scene* newScene = mLinkedPortal->mSceneToRender;
				Application::SetActiveScene(newScene);

				// move the camera in the new scene to the position through the portal
				Camera* cam = newScene->GetActiveCamera();

				// adjust it slightly so we dont end up inside the portal again
				cam->setPosition(traveller->getPosition() + 0.1f * traveller->getMoveDirection() - mTransform.GetTranslation() + mLinkedPortal->mTransform.GetTranslation());
				cam->setPitch(traveller->getPitch());
				cam->setYaw(traveller->getYaw());
			}
		}
	}
	sLastSideOfPortal = sideOfPortal;

	sPortalUpdateInProgress = false;
}

void Portal::Render()
{
	// make sure we are rendering portals only in the portal pass
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
		// note: rotations do not work. do not rotate portals. you will see nothing through them but pain and suffering
		glm::quat q(m);
		glm::vec3 euler = glm::eulerAngles(q);

		// get distance from pos to portal
		glm::vec3& a = mSceneToRender->GetActiveCamera()->getPosition() - GetTransform().GetTranslation();
		float distanceToPortal = glm::length(glm::vec2(a.x, a.z));


		// mostly for peace of mind that we wont mess up the opengl state by rendering another scene within this one
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		{
			Transformation t(pos, euler, { 1, 1, 1 });

			// we need to render the skybox at a specific position
			const Skybox* s = mLinkedPortal->mSceneToRender->GetSkybox();
			if (s != nullptr)
			{
				s->render(mSceneToRender->GetActiveCamera()->getPosition() - mTransform.GetTranslation() + mLinkedPortal->mTransform.GetTranslation());
			}

			// render the scene that the linked portal looks into


			// work out where to place the near clipping plane such that nothing on the far side of the portal is clipped,
			// but everything on the near side of the portal is clipped
			Camera* cam = mSceneToRender->GetActiveCamera();
			
			glm::vec3 d{ glm::normalize(mTransform.GetTranslation() - cam->getPosition()) };
			glm::vec3 f{ cam->getForward() };
			float angle = glm::dot(d, f);

			Scene::setNearPlane(glm::max(0.05f, angle * distanceToPortal));
			
			// render the other scene, but without its skybox as it has already been rendered
			Skybox::DisableSkyboxRendering();
			mLinkedPortal->mSceneToRender->RenderSceneLights();
			mLinkedPortal->mSceneToRender->OnRenderObjects();
			Skybox::EnableSkyboxRendering();
			
			// reset the near plane
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
		
		// save the material that is currently applied
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

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

	// finally draw the frame of the portal just like a regular object
	Material::Default.apply();
	{
		Transformation t(mTransform);
		{
			// the adjustment is required to stop z-fighting against the interior of the portal and the floor
			const float scaleAdjustment = 0.001f;
			Transformation adjustement({ 0, 0, 0 }, { 0, 0, 0 }, glm::vec3{ 1 - scaleAdjustment });
			RenderHelper::drawMesh(mFrameModel);
		}
	}

	glPopAttrib();


	// portal render is finished
	sPortalRenderInProgress = false;
}
