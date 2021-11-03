#include "Skybox.h"


#include "Transform.h"
#include "GeometryHelper.h"

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
	}
}

Skybox::~Skybox()
{
}

void Skybox::render(const Vector3& position)
{
	// set up opengl state
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// bind texture
	mTexture.Bind();

	// move to the given position
	Transform t{ position };
	
	glBegin(GL_TRIANGLES);

	for (auto& face : mFaceData)
	{
		FaceData& fD = face.second;


		
		for (auto& i : fD.planeMesh.Indices)
		{

			auto& v = fD.planeMesh.Vertices[i];

			Vector2 adjustedTexCoord = v.TexCoord + fD.uvOffset;

			glTexCoord2fv(adjustedTexCoord.ptr());
			glNormal3fv(v.Normal.ptr());
			glVertex3fv(v.Position.ptr());

		}


	}

	glEnd();

	// FaceData face;
	// 
	// 
	// glBegin(GL_QUADS);
	// 
	// // right face
	// face = mFaceData[Face::Right];
	// glNormal3fv(face.normal.ptr());
	// 
	// glTexCoord2fv(face.topleftUV.ptr());
	// glVertex3f(1, 1, -1);
	// glTexCoord2f(face.topleftUV.x, face.bottomRightUV.y);
	// glVertex3f(1, -1, -1);
	// glTexCoord2fv(face.bottomRightUV.ptr());
	// glVertex3f(1, -1,  1);
	// glTexCoord2f(face.bottomRightUV.x, face.topleftUV.y);
	// glVertex3f(1, 1,  1);
	// 
	// glEnd();


	// revert opengl state
	glPopAttrib();
}
