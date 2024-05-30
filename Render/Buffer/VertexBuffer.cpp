#include "Framework.h"
#include "VertexBuffer.h"

VertexBuffer::~VertexBuffer()
{
    vertexBuffer->Release();
}

void VertexBuffer::IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY type)
{
    DC->IASetPrimitiveTopology(type);
    DC->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
}

void VertexBuffer::IASetBuffer(UINT slot, D3D11_PRIMITIVE_TOPOLOGY type)
{
    DC->IASetPrimitiveTopology(type);
    DC->IASetVertexBuffers(slot, 1, &vertexBuffer, &stride, &offset);
}

void VertexBuffer::UpdateVertex(void* data, UINT count)
{
    DC->UpdateSubresource(vertexBuffer, 0, nullptr, data, stride, count);
}
