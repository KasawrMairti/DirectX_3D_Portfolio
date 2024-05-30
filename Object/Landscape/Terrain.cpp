#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(wstring diffuseFile, wstring heightFile)
{
	material = new Material();
	material->SetShader(L"09Light");
	material->SetDiffuseMap(diffuseFile);

	heightMap = Texture::Get(heightFile);

	CreateMesh();
	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices, indices);
}

Terrain::~Terrain()
{
	delete mesh;
	delete material;
}

void Terrain::Render()
{
	Transform::SetWorld();

	    mesh->SetMesh();
	material->SetMaterial();

	DC->DrawIndexed(indices.size(), 0, 0);
}

float Terrain::GetHeight(Vector3 position)
{
	position.x /= scale.x;
	position.z /= scale.z;

	int x = (int)position.x;
	int z = (int)position.z;

	if (x < 0 || x >= width - 1 || z < 0 || z >= height - 1) return 0.0f;

	UINT index[4];
	index[0] = x + 0 + width * (z + 1);
	index[1] = x + 1 + width * (z + 1);
	index[2] = x + 0 + width * (z + 0);
	index[3] = x + 1 + width * (z + 0);

	Vector3 vertex[4];
	for (UINT i = 0; i < 4; i++)
	{
		vertex[i] = vertices[index[i]].pos;
	}

	float u = abs(position.x - vertex[0].x);
	float v = abs(position.z - vertex[0].z);

	Vector3 result;

	if (u + v <= 1.0f)
	{
		result = vertex[0] + (vertex[2] - vertex[0]) * u + (vertex[1] - vertex[0]) * v;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = vertex[3] + (vertex[2] - vertex[3]) * u + (vertex[1] - vertex[3]) * v;
	}

	return result.y;
}

void Terrain::CreateMesh()
{
     width = heightMap->GetSize().x;
	height = heightMap->GetSize().y;

	vector<Vector4> colors = heightMap->ReadPixels();

	//Vertices
	for (float z = 0; z < height; z++)
	{
		for (float x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.pos = Vector3(x, 0, z);

			vertex.uv.x =     x / (width  - 1);
			vertex.uv.y = 1 - z / (height - 1);

			//HeightMap
			UINT index = x + z * width;
			vertex.pos.y = colors[index].x * MAP_HEIGHT;


			vertices.push_back(vertex);
		}
	}

	//Indices
	for (float z = 0; z < height - 1; z++)
	{
		for (float x = 0; x < width - 1; x++)
		{
			indices.push_back(x + 0 + width * (z + 1)); //7
			indices.push_back(x + 1 + width * (z + 1)); //8
			indices.push_back(x + 0 + width * (z + 0)); //0

			indices.push_back(x + 0 + width * (z + 0)); //0
			indices.push_back(x + 1 + width * (z + 1)); //8
			indices.push_back(x + 1 + width * (z + 0)); //1
		}
	}

}

void Terrain::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];
		
		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector3 v01 = p1 - p0;
		Vector3 v02 = p2 - p0;

		Vector3 normal = Vector3::Cross(v01, v02).GetNormalized();

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}
}

void Terrain::CreateTangent()
{
	for (VertexType& vertex : vertices)
		vertex.tangent = Vector3();

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
