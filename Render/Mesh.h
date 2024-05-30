#pragma once

class Mesh
{
public:
	template <typename T>
	Mesh(vector<T>& vertices, vector<UINT>& indices);
	~Mesh();

	void SetMesh(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	void UpdateVertex(void* data, UINT count) { vertexBuffer->UpdateVertex(data, count); }

private:
	VertexBuffer* vertexBuffer;
	 IndexBuffer*  indexBuffer;
};

template<typename T>
inline Mesh::Mesh(vector<T>& vertices, vector<UINT>& indices)
{
	vertexBuffer = new VertexBuffer(vertices);
	 indexBuffer = new  IndexBuffer( indices);
}
