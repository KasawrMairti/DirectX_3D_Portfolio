#include "Framework.h"
#include "Quad.h"

Quad::Quad(Vector2 size)
{
	float L = -size.x * 0.5f;
	float R = +size.x * 0.5f;
	float B = -size.y * 0.5f;
	float T = +size.y * 0.5f;

	vertices =
	{
		{ Vector3(L, T, 0.0f), Vector2(0, 0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(R, T, 0.0f), Vector2(1, 0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(L, B, 0.0f), Vector2(0, 1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(R, B, 0.0f), Vector2(1, 1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
	};

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	mesh = new Mesh(vertices, indices);

	material = new Material();
	material->SetShader(L"09Light");
}

Quad::Quad(wstring file)
{
	material = new Material();
	material->SetShader(L"09Light");
	material->SetDiffuseMap(file);
	
	Vector2 size = material->GetDiffuseMap()->GetSize();
	float L = -size.x * 0.5f;
	float R = +size.x * 0.5f;
	float B = -size.y * 0.5f;
	float T = +size.y * 0.5f;

	vertices =
	{
		{ Vector3(L, T, 0.0f), Vector2(0, 0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(R, T, 0.0f), Vector2(1, 0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(L, B, 0.0f), Vector2(0, 1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
		{ Vector3(R, B, 0.0f), Vector2(1, 1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f) },
	};

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	mesh = new Mesh(vertices, indices);
}

Quad::~Quad()
{
	delete mesh;
}

void Quad::Render()
{
	if (!isActive) return;

	Transform::SetWorld();

	material->SetMaterial();
	    mesh->SetMesh();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Quad::RenderInstanced(UINT instanceCount)
{
	Transform::SetWorld();

	material->SetMaterial();
	    mesh->SetMesh();

	DC->DrawIndexedInstanced(indices.size(), instanceCount, 0, 0, 0);
}
