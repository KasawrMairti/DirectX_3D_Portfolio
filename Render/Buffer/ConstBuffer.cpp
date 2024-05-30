#include "Framework.h"
#include "ConstBuffer.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
	:data(data), dataSize(dataSize)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	
	bufferDesc.ByteWidth           = dataSize;
	bufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags           = 0;
	bufferDesc.StructureByteStride = 0;
	
	DEVICE->CreateBuffer(&bufferDesc, nullptr, &constBuffer);
}

ConstBuffer::~ConstBuffer()
{
	constBuffer->Release();
}

void ConstBuffer::SetVSBuffer(UINT slot)
{
	UpdateSubResource();

	DC->VSSetConstantBuffers(slot, 1, &constBuffer);
}

void ConstBuffer::SetPSBuffer(UINT slot)
{
	UpdateSubResource();

	DC->PSSetConstantBuffers(slot, 1, &constBuffer);
}

void ConstBuffer::SetCSBuffer(UINT slot)
{
	UpdateSubResource();

	DC->CSSetConstantBuffers(slot, 1, &constBuffer);
}

void ConstBuffer::UpdateSubResource()
{
	//Map, Unmap

	DC->Map(constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	memcpy(subResource.pData, data, dataSize);

	DC->Unmap(constBuffer, 0);

	//DC->UpdateSubresource(constBuffer, 0, nullptr, data, 0, 0);
}
