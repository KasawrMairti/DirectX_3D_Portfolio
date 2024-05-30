#include "Framework.h"
#include "Sphere.h"

Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	:radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	CreateMesh();
	CreateTangent();

	mesh = new Mesh(vertices, indices);

	material = new Material();
	material->SetShader(L"09Light");
}

Sphere::~Sphere()
{
	delete mesh;
	delete material;
}

void Sphere::Render()
{
	Transform::SetWorld();

	material->SetMaterial();
	    mesh->SetMesh();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Sphere::CreateMesh()
{
	for (UINT i = 0; i < stackCount + 1; i++)
	{
		float phi = XM_PI / stackCount * i;

		for (UINT j = 0; j < sliceCount + 1; j++)
		{
			float theta = XM_2PI / sliceCount * j;

			VertexType vertex;

			vertex.pos.x = radius * sin(phi) * cos(theta);
			vertex.pos.y = radius * cos(phi);
			vertex.pos.z = radius * sin(phi) * sin(theta);

			vertex.normal = vertex.pos / radius;

			vertex.uv.x = j / (float)sliceCount;
			vertex.uv.y = i / (float)stackCount;

			vertices.push_back(vertex);
		}
	}

	for (UINT j = 0; j < stackCount; j++)
	{
		for (UINT i = 0; i < sliceCount; i++)
		{
			indices.push_back(i + 0 + (sliceCount + 1) * (j + 0));
			indices.push_back(i + 1 + (sliceCount + 1) * (j + 0));
			indices.push_back(i + 0 + (sliceCount + 1) * (j + 1));

			indices.push_back(i + 0 + (sliceCount + 1) * (j + 1));
			indices.push_back(i + 1 + (sliceCount + 1) * (j + 0));
			indices.push_back(i + 1 + (sliceCount + 1) * (j + 1));
		}
	}
}

void Sphere::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];
		
		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector2 uv0 = vertices[index0].uv;
		Vector2 uv1 = vertices[index1].uv;
		Vector2 uv2 = vertices[index2].uv;

		Vector3 e01 = p1 - p0;
		Vector3 e02 = p2 - p0;

		float u1 = uv1.x - uv0.x;
		float v1 = uv1.y - uv0.y;

		float u2 = uv2.x - uv0.x;
		float v2 = uv2.y - uv0.y;

		float D = 1.0f / (u1 * v2 - v1 * u2);

		Vector3 tangent = D * (e01 * v2 - e02 * v1);

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 T = vertex.tangent;
		Vector3 N = vertex.normal;

		vertex.tangent = (T - N * Vector3::Dot(N, T)).GetNormalized();
	}
}
