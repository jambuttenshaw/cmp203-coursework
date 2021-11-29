#include "ShadowHelper.h"

#include <array>
#include <unordered_map>

void ShadowHelper::CalculateShadowMatrix(glm::mat4& out, const glm::vec3& lightPos, const glm::vec3& planeNormal, const glm::vec3& pointOnPlane)
{
	float a = planeNormal.x;
	float b = planeNormal.y;
	float c = planeNormal.z;
	float d = glm::dot(planeNormal, pointOnPlane);

	float x = lightPos.x;
	float y = lightPos.y;
	float z = lightPos.z;

	out[0][0] = d - (b * y + c * z);
	out[0][1] = b * x;
	out[0][2] = c * x;
	out[0][3] = -d * x;

	out[1][0] = a * y;
	out[1][1] = d - (a * x + c * z);
	out[1][2] = c * y;
	out[1][3] = -d * y;

	out[2][0] = a * z;
	out[2][1] = b * z;
	out[2][2] = d - (a * x + b * y);
	out[2][3] = -d * z;

	out[3][0] = a;
	out[3][1] = b;
	out[3][2] = c;
	out[3][3] = d - (a * x + b * y + c * z);
}

Mesh ShadowHelper::BuildShadowVolume(const Mesh& mesh, const glm::vec3& lightPos)
{
	const float extension = 20.0f;

	auto edges = CalculateSilhouette(mesh, lightPos);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// to map from an index in the parameter mesh to the shadow volume mesh
	std::unordered_map<size_t, size_t> indexMapping;

	// build a quad for each edge
	for (auto& edge : edges)
	{
		size_t a, b, c, d; // the four indices of the new quad

		// check if weve seen the vertex before
		if (indexMapping.find(edge.first) != indexMapping.end())
		{
			// we have!
			a = indexMapping[edge.first];
			b = a + 1;
		}
		else
		{
			// we have not
			
			// get the new indices 
			a = vertices.size();
			b = vertices.size() + 1;

			// add the new vertices to the shadow volume
			Vertex v = mesh.Vertices[edge.first];
			vertices.push_back(v);
			v.Position = ExtendVertex(v.Position, lightPos, extension);
			vertices.push_back(v);

			// insert into the mapping
			indexMapping.insert({ edge.first, a });
		}


		//  repeat for c and d
		if (indexMapping.find(edge.second) != indexMapping.end())
		{
			c = indexMapping[edge.second];
			d = c + 1;
		}
		else
		{
			c = vertices.size();
			d = vertices.size() + 1;

			Vertex v = mesh.Vertices[edge.second];
			vertices.push_back(v);
			v.Position = ExtendVertex(v.Position, lightPos, extension);
			vertices.push_back(v);

			indexMapping.insert({ edge.second, c });
		}


		// now create the 2 triangles that form this quad
		indices.push_back(static_cast<unsigned int>(a));
		indices.push_back(static_cast<unsigned int>(b));
		indices.push_back(static_cast<unsigned int>(d));

		indices.push_back(static_cast<unsigned int>(a));
		indices.push_back(static_cast<unsigned int>(d));
		indices.push_back(static_cast<unsigned int>(c));
	}

	return Mesh(vertices, indices);
}

glm::vec3 ShadowHelper::ExtendVertex(const glm::vec3& pos, const glm::vec3& lightPos, float extension)
{
	glm::vec3 dir = glm::normalize(pos - lightPos);
	return pos + extension * dir;
}

std::set<std::pair<size_t, size_t>> ShadowHelper::CalculateSilhouette(const Mesh& mesh, const glm::vec3& lightPos)
{
	std::set<std::pair<size_t, size_t>> silhouetteEdges;
	std::array<std::pair<size_t, size_t>, 3> edges;

	// loop through all triangles in the mesh
	for (size_t triangle = 0; triangle < mesh.Indices.size(); triangle += 3)
	{
		size_t indexA = mesh.Indices[triangle];
		size_t indexB = mesh.Indices[triangle + 1];
		size_t indexC = mesh.Indices[triangle + 2];

		glm::vec3 a = mesh.Vertices[indexA].Position;
		glm::vec3 b = mesh.Vertices[indexB].Position;
		glm::vec3 c = mesh.Vertices[indexC].Position;

		// get the normal of the triangle
		glm::vec3 normal = glm::cross(b - a, c - a);

		if (glm::dot(normal, lightPos - a) > 0)
		{
			edges[0] = std::make_pair(indexA, indexB);		// ab
			edges[1] = std::make_pair(indexB, indexC);		// bc
			edges[2] = std::make_pair(indexC, indexA);		// ca

			for (const auto& edge : edges)
			{
				// have we already found this edge?
				if (silhouetteEdges.find(edge) != silhouetteEdges.end())
				{
					// yes; remove it from the set
					silhouetteEdges.erase(edge);
				}
				// have we already found the reverse of this edge?
				else
				{
					auto reverse = std::make_pair(edge.second, edge.first);
					if (silhouetteEdges.find(reverse) != silhouetteEdges.end())
					{
						// yes; remove it from the set
						silhouetteEdges.erase(reverse);
					}
					else
					{
						// neither this edge or its reverse have been found before
						// insert it into the set
						silhouetteEdges.insert(edge);
					}
				}
			}
		}
	}

	return silhouetteEdges;
}
