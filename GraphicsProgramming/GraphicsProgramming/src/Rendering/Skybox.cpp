#include "Skybox.h"


#include "Transform.h"
#include "GeometryHelper.h"
#include "RenderHelper.h"

#include "freeglut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


Skybox::Skybox(const std::string& filepath)
{
	// unlike a regular mesh, a skybox manages its own texture
	MeshTexture = new Texture(filepath);
	Rebuild();
}

Skybox::~Skybox()
{
	// unlike a regular mesh, a skybox manages its own texture
	delete MeshTexture;
}

void Skybox::render(const Vector3& position)
{
	// set up opengl state
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// move to the given position
	Transform t{ position };
	
	RenderHelper::drawMesh(*this);

	// revert opengl state
	glPopAttrib();
}

void Skybox::Rebuild()
{
	// Remember: skybox is an inside-out cube, so normals will be opposite from what you expect!
	struct FaceData
	{
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
		Vector2 uvOffset;
	};
	
	FaceData faceData[6] = {
		{
			{ -1,  0, 0 },
			{  0,  0, 1 },
			{  0,  1, 0 },
			{ 0.5f, 0.25f }
		},
		{
			{ 1,  0,  0 },
			{ 0,  0, -1 },
			{ 0,  1,  0 },
			{ 0.0f, 0.25f }
		},
		{
			{ 0, -1,  0 },
			{ 1,  0,  0 },
			{ 0,  0,  1 },
			{ 0.25f, 0.0f }
		},
		{
			{ 0, 1,  0 },
			{ 1, 0,  0 },
			{ 0, 0, -1 },
			{ 0.25f, 0.5f }
		},
		{
			{ 0,  0, 1 }, // the front of the skybox points along +ve z
			{ 1,  0, 0 },
			{ 0,  1, 0 },
			{ 0.25f, 0.25f }
		},
		{
			{  0,  0, -1 }, // the back of the skybox points along -ve z
			{ -1,  0,  0 },
			{  0,  1,  0 },
			{ 0.75f, 0.25f }
		}
	};

	for (auto& face : faceData)
	{
		Mesh faceMesh = GeometryHelper::CreatePlane(2, 2, face.normal, 0.25f, 0.25f,
			[](float x, float y) -> float { return -0.5f; },
			face.tangent, face.bitangent);

		for (auto& vertex : faceMesh.Vertices)
		{
			vertex.TexCoord += face.uvOffset;
		}

		GeometryHelper::CombineMeshes(*this, faceMesh);
	}
}
