#include "Portal.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <glm/gtx/transform.hpp>

#include "Core/Scene.h"

#include "Rendering/GeometryHelper.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/Camera.h"

#include <cassert>


bool Portal::sPortalRenderInProgress = false;

Portal::Portal(Scene* sceneToRender)
{
	mFrameModel = GeometryHelper::LoadObj("models/portal.obj");
	mScreenModel = GeometryHelper::LoadObj("models/portalScreen.obj");

	mSceneToRender = sceneToRender;
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
		
		glm::mat4 m = mLinkedPortal->GetTransform().LocalToWorld() * mTransform.WorldToLocal() *
			mSceneToRender->GetActiveCamera().getLocalToWorldMatrix();
		glm::vec3 up = m[1];
		glm::vec3 pos = m[3];
		// look at = pos + forward
		glm::vec3 lookAt = pos + glm::vec3(m[2]);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		{
			Transformation t;
			gluLookAt(pos.x,	pos.y,	  pos.z, 
					  lookAt.x, lookAt.y, lookAt.z, 
					  up.x,		up.y,	  up.z);

			// render the scene that the linked portal looks into
			mLinkedPortal->mSceneToRender->OnRender();
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
