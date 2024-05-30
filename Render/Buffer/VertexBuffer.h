#pragma once

class VertexBuffer
{
public:
	template <typename T>
	VertexBuffer(vector<T>& vertices);
	~VertexBuffer();

	void IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void IASetBuffer(UINT slot = 0, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    void UpdateVertex(void* data, UINT count);

private:
	ID3D11Buffer* vertexBuffer;

    UINT stride = 0;
    UINT offset = 0;
};

template<typename T>
inline VertexBuffer::VertexBuffer(vector<T>& vertices)
{
    stride = sizeof(T);
    offset = 0;

    D3D11_BUFFER_DESC bufferDesc = {};

    bufferDesc.ByteWidth           = stride * vertices.size();
    bufferDesc.Usage               = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags      = 0;
    bufferDesc.MiscFlags           = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;

    data.pSysMem = vertices.data();

    DEVICE->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
}
