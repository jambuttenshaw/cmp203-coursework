#include "Skybox.h"


#include "Transform.h"
#include "GeometryHelper.h"
#include "RenderHelper.h"

#include "freeglut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


Skybox::Skybox(const std::string& filepath)
	: mTexture(filepath)
{
	// Remember: skybox is an inside-out cube, so normals will be opposite from what you expect!
	mFaceData[Face::Right] =
	{
		{ -1,  0, 0 },
		{  0,  0, 1 },
		{  0,  1, 0 },
		{ 0.5f, 0.25f }
	};
	mFaceData[Face::Left] =
	{
		{ 1,  0,  0 },
		{ 0,  0, -1 },
		{ 0,  1,  0 },
		{ 0.0f, 0.25f }
	};
	mFaceData[Face::Top] =
	{
		{ 0, -1,  0 },
		{ 1,  0,  0 },
		{ 0,  0,  1 },
		{ 0.25f, 0.0f }
	};
	mFaceData[Face::Bottom] =
	{
		{ 0, 1,  0 },
		{ 1, 0,  0 },
		{ 0, 0, -1 },
		{ 0.25f, 0.5f }
	};
	mFaceData[Face::Front] =
	{
		{ 0,  0, 1 }, // the front of the skybox points along +ve z
		{ 1,  0, 0 },
		{ 0,  1, 0 },
		{ 0.25f, 0.25f }
	};
	mFaceData[Face::Back] =
	{
		{  0,  0, -1 }, // the back of the skybox points along -ve z
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{ 0.75f, 0.25f }
	};

	for (auto& face : mFaceData)
	{
		FaceData& fD = face.second;
		fD.planeMesh = GeometryHelper::CreatePlane(2, 2, fD.normal, 0.25f, 0.25f, 
			[](float x, float y) -> float { return -0.5f; },
			fD.tangent, fD.bitangent);

		for (auto& vertex : fD.planeMesh.Vertices)
		{
			vertex.TexCoord += fD.uvOffset;
		}
	}
}

void Skybox::setFaceUVOffset(Face face, const Vector2& offset)
{
	if (face == Face::None) return;
	if (face == Face::All)
	{
		for (auto& face : mFaceData) face.second.uvOffset = offset;
	}
	else
	{
		mFaceData[face].uvOffset = offset;
	}
}

void Skybox::setFaceUVDimensions(Face face, const Vector2& size)
{
	if (face == Face::None) return;
	if (face == Face::All)
	{
		for (auto& face : mFaceData)
		{
			FaceData& fD = face.second;
			fD.planeMesh = GeometryHelper::CreatePlane(2, 2, fD.normal, size.x, size.y,
				[](float x, float y) -> float { return -0.5f; },
				fD.tangent, fD.bitangent);
		}
	}
	else
	{
		FaceData& fD = mFaceData[face];
		fD.planeMesh = GeometryHelper::CreatePlane(2, 2, fD.normal, size.x, size.y,
			[](float x, float y) -> float { return -0.5f; },
			fD.tangent, fD.bitangent);
	}
}

void Skybox::render(const Vector3& position)
{
	// set up opengl state
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// bind texture
	mTexture.Bind();

	// move to the given position
	Transform t{ position };
	
	for (auto& face : mFaceData)
		RenderHelper::drawMesh(face.second.planeMesh);

	// unbind texture
	mTexture.Unbind();


	// revert opengl state
	glPopAttrib();
}
