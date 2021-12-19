#include "TransparentIcosahedron.h"

#include "Core/Random.h"

#include "Rendering/GeometryHelper.h"
#include "Rendering/RenderHelper.h"

#include <algorithm>
#include <freeglut.h>
#include <gl/GL.h>
#include <gl/GLU.h>


TransparentIcosahedron::TransparentIcosahedron()
{
	// start with an icosahedron mesh
	Mesh icosahedron = GeometryHelper::CreateIcosahedron();

	// the mesh data needs to be restructured so each face can be sorted and coloured differently
	size_t index = 0;
	for (size_t i = 0; i < 20; i++)
	{
		Face newFace;

		newFace.mesh.Indices = { 0, 1, 2 };
		glm::vec3 centroid{ 0.0f };
		for (size_t j = index; j < index + 3; j++)
		{
			Vertex v = icosahedron.Vertices[icosahedron.Indices[j]];
			newFace.mesh.Vertices.push_back(v);
			centroid += v.Position;
		}
		index += 3;
		
		centroid /= 3.0f;
		newFace.centroid = centroid;

		newFace.color = Color{ glm::vec4(Random::RandomUnitVector3(), 0.5f) };
		// newFace.color = centroid.x > 0 ? Color{0, 0, 1, 0.5f} : Color{ 0, 1, 0, 0.5f };

		faces[i] = newFace;
	}
}

void TransparentIcosahedron::Render(const glm::vec3& camPos)
{
	// sort faces according to cam pos
	std::sort(faces.begin(), faces.end(), [this, camPos](Face a, Face b) -> bool {
			glm::vec3 toA = glm::vec4(camPos, 1.0f) - gameObject.GetTransform().LocalToWorld() * glm::vec4(a.centroid, 1.0f);
			glm::vec3 toB = glm::vec4(camPos, 1.0f) - gameObject.GetTransform().LocalToWorld() * glm::vec4(b.centroid, 1.0f);
			// glm::vec3 toA = camPos - a.centroid;
			// glm::vec3 toB = camPos - b.centroid;
			return glm::dot(toA, toA) > glm::dot(toB, toB);
		});

	glPushAttrib(GL_LIGHTING_BIT);

	for (const auto& face : faces)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, face.color.ptr());
		RenderHelper::drawMesh(face.mesh);
	}

	glPopAttrib();
}
