#include "Skybox.h"


#include "Transformation.h"
#include "GeometryHelper.h"
#include "RenderHelper.h"

#include "freeglut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


bool Skybox::sSkyboxesEnabled = true;

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

void Skybox::render(const glm::vec3& position) const
{
	if (!sSkyboxesEnabled) return;

	// set up opengl state
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// move to the given position
	Transformation t{ position };
	
	// skybox then draws itself
	RenderHelper::drawMesh(*this);

	// revert opengl state
	glPopAttrib();
}

void Skybox::Rebuild()
{
	// Remember: skybox is an inside-out cube, so normals will be opposite from what you expect!
	// FaceData contains everything you need to construct a face of the skybox
	struct FaceData
	{
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 uvOffset;
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
		// create a plane mesh from the face data
		Mesh faceMesh = GeometryHelper::CreatePlane(2, 2, face.normal, 0.25f, 0.25f,
			[](float x, float y) -> float { return -0.5f; },
			face.tangent, face.bitangent);

		// apply the offset to the texture coordinates
		for (auto& vertex : faceMesh.Vertices)
		{
			vertex.TexCoord += face.uvOffset;
		}

		// add the new face onto itself
		GeometryHelper::CombineMeshes(*this, faceMesh);
	}
}
