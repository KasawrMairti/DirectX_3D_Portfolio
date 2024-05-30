#include "Framework.h"
#include "Mesh.h"

Mesh::~Mesh()
{
	delete vertexBuffer;
	delete  indexBuffer;
}

void Mesh::SetMesh(D3D11_PRIMITIVE_TOPOLOGY type)
{
	vertexBuffer->IASetBuffer(type);
     indexBuffer->IASetBuffer();
}
