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

	// the mesh data needs to be seperated into an array of meshes, one for each triangle,
	// so each face can be sorted and coloured differently
	size_t index = 0;
	for (size_t i = 0; i < 20; i++)
	{
		// create a new face
		Face newFace;

		// each mesh has only 1 face formed of 3 vertices,
		// the indices are identical for each mesh
		newFace.mesh.Indices = { 0, 1, 2 };
		// centroid is the mean position of all vertices on the triangle
		// and is used for working out which face is closest to the camera
		glm::vec3 centroid{ 0.0f };
		for (size_t j = index; j < index + 3; j++)
		{
			// get the correct vertex from the base mesh and add it to this face's mesh
			Vertex v = icosahedron.Vertices[icosahedron.Indices[j]];
			newFace.mesh.Vertices.push_back(v);
			// add the position to our centroid calculation
			centroid += v.Position;
		}
		index += 3;
		
		// calculate centroid
		centroid /= 3.0f;
		newFace.centroid = centroid;

		// assign the face a random colour
		newFace.color = Color{ glm::vec4(Random::RandomUnitVector3(), faceAlpha) };

		// and assign the face into the array
		faces[i] = newFace;
	}
}

void TransparentIcosahedron::Render(const glm::vec3& camPos)
{
	// sort faces according to cam pos
	std::sort(faces.begin(), faces.end(), [this, camPos](Face a, Face b) -> bool {
			// work out the vector from each face to the camera in world space
			glm::vec3 toA = glm::vec4(camPos, 1.0f) - gameObject.GetTransform().LocalToWorld() * glm::vec4(a.centroid, 1.0f);
			glm::vec3 toB = glm::vec4(camPos, 1.0f) - gameObject.GetTransform().LocalToWorld() * glm::vec4(b.centroid, 1.0f);
			// compare the square magnitude of each vector
			return glm::dot(toA, toA) > glm::dot(toB, toB);
		});

	// we want to override the current material with whatever the faces colour has been assigned
	glPushAttrib(GL_LIGHTING_BIT);

	for (const auto& face : faces)
	{
		// assign the colour
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, face.color.ptr());
		// draw the mesh
		RenderHelper::drawMesh(face.mesh);
	}

	glPopAttrib();
}

void TransparentIcosahedron::RandomizeColours()
{
	// assign a random colour to each face
	for (auto& face : faces)
		face.color = Color{ glm::vec4(Random::RandomUnitVector3(), faceAlpha) };
}
